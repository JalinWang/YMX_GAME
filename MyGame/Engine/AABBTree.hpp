#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
using namespace std;

#include "Common.hpp"

#define MARGINE 50.0
struct Position;
struct BaseObject;
struct AABB;
struct AABBTreeNode;
AABB Union(const AABB& a, const AABB& b);
typedef pair<AABB*, AABB*> ColliderPair;
typedef vector<ColliderPair> ColliderPairList;
inline ColliderPair AllocatePair(AABB* a, AABB* b)
{
	return make_pair(a, b);
}

struct AABB {
	Position minPoint, maxPoint;
	AABBTreeNode* userData;
	void* pObj;
	//BaseObject* obj;

	AABB() :userData(nullptr), pObj(nullptr) {}
	//AABB(BaseObject* o) :obj(o), userData(nullptr)
	//{
	//	if (o->GetShapeType() == 0)
	//	{
	//		minPoint = Position(o->GetPos() - o->GetRadius());
	//		maxPoint = Position(o->GetPos() + o->GetRadius());
	//	}
	//	else
	//	{
	//		minPoint = Position(o->GetPos().x - o->GetWidth() / 2, o->GetPos().y - o->GetHeight() / 2);
	//		maxPoint = Position(o->GetPos().x + o->GetWidth() / 2, o->GetPos().y + o->GetHeight() / 2);
	//	}
	//}

	bool Contains(const AABB& t) const
	{
		return minPoint < t.minPoint && t.maxPoint < maxPoint;
	}

	bool Contains(const Position& t) const
	{
		return minPoint < t && t < maxPoint;
	}

	friend bool Collides(const AABB& a, const AABB& b)
	{
		if (a.Contains(b.minPoint) || a.Contains(b.maxPoint)
			|| b.Contains(a.minPoint) || b.Contains(a.maxPoint))
			return true;
		else
			return false;
	}

	friend AABB Union(const AABB& a, const AABB& b)
	{
		AABB ret;
		ret.minPoint.x = min(a.minPoint.x, b.minPoint.x);
		ret.minPoint.y = min(a.minPoint.y, b.minPoint.y);
		ret.maxPoint.x = max(a.maxPoint.x, b.maxPoint.x);
		ret.maxPoint.y = max(a.maxPoint.y, b.maxPoint.y);
		return ret;
	}

	double Area() const
	{
		return (maxPoint.x - minPoint.x) * (maxPoint.y - minPoint.y);
	}
};

struct AABBTreeNode
{
	AABBTreeNode* parent;
	AABBTreeNode* children[2];

	// these will be explained later
	bool childrenCrossed;
	AABB aabbFat;
	AABB* data;

	AABBTreeNode() :parent(nullptr), data(nullptr)
	{
		children[0] = nullptr;
		children[1] = nullptr;
	}

	bool IsLeaf() const
	{
		return (children[0] == nullptr);
	}

	// make this ndoe a branch
	void SetBranch(AABBTreeNode* n0, AABBTreeNode* n1)
	{
		n0->parent = this;
		n1->parent = this;

		children[0] = n0;
		children[1] = n1;
	}

	// make this node a leaf
	void SetLeaf(AABB* data)
	{
		// create two-way link
		this->data = data;
		data->userData = this;

		children[0] = nullptr;
		children[1] = nullptr;
	}

	void UpdateAABB()
	{
		if (IsLeaf())
		{
			// make fat AABB
			aabbFat.minPoint = data->minPoint - MARGINE;
			aabbFat.maxPoint = data->maxPoint + MARGINE;
		}
		else
			// make union of child AABBs of child nodes
			aabbFat = Union(children[0]->aabbFat, children[1]->aabbFat);
	}

	AABBTreeNode* GetSibling() const
	{
		return this == parent->children[0] ? parent->children[1] : parent->children[0];
	}
};

struct AABBTree{
	int cnt;
	AABBTree() :m_root(nullptr), cnt(0) {}

	void AABBTreeUpdate()
	{
		if (m_root)
		{
			if (m_root->IsLeaf())
				m_root->UpdateAABB();
			else
			{
				// grab all invalid nodes
				_invalidNodes.clear();
				UpdateNodeHelper(m_root);

				// re-insert all invalid nodes
				for (AABBTreeNode* node : _invalidNodes)
				{
					// grab parent link
					// (pointer to the pointer that points to parent)
					AABBTreeNode* parent = node->parent;
					AABBTreeNode* sibling = node->GetSibling();
					AABBTreeNode** parentLink =
						parent->parent
						? (parent == parent->parent->children[0]
							? &parent->parent->children[0]
							: &parent->parent->children[1])
						: &m_root;

					// replace parent with sibling
					sibling->parent =
						parent->parent
						? parent->parent
						: nullptr; // root has null parent

					*parentLink = sibling;
					delete parent;

					// re-insert node
					node->UpdateAABB();
					InsertNode(node, &m_root);
				}
				_invalidNodes.clear();
			}
		}
	}

	void Add(AABB* aabb)
	{
		++cnt;
		if (m_root)
		{
			// not first node, insert node to tree
			AABBTreeNode* node = new AABBTreeNode();
			node->SetLeaf(aabb);
			node->UpdateAABB();
			InsertNode(node, &m_root);
		}
		else
		{
			// first node, make root
			m_root = new AABBTreeNode();
			m_root->SetLeaf(aabb);
			m_root->UpdateAABB();
		}
	}

	void Remove(AABB* aabb)
	{
		--cnt;
		if (aabb->userData == nullptr)
			terminate();
		AABBTreeNode* node = aabb->userData;

		// remove two-way link
		node->data = nullptr;
		aabb->userData = nullptr;

		RemoveNode(node);
	}

	ColliderPairList& ComputePairs()
	{
		m_pairs.clear();

		// early out
		if (!m_root || m_root->IsLeaf())
			return m_pairs;

		// clear AABBTreeNode::childrenCrossed flags
		ClearChildrenCrossFlagHelper(m_root);

		// base recursive call
		ComputePairsHelper(m_root->children[0], m_root->children[1]);

		return m_pairs;
	}
private:

	AABBTreeNode* m_root;
	vector<AABBTreeNode*> _invalidNodes;
	ColliderPairList m_pairs;

	void UpdateNodeHelper(AABBTreeNode* node)
	{
		if (node->IsLeaf())
		{
			// check if fat AABB doesn't 
			// contain the collider's AABB anymore
			if (!node->aabbFat.Contains(*(node->data)))
				_invalidNodes.push_back(node);
		}
		else
		{
			UpdateNodeHelper(node->children[0]);
			UpdateNodeHelper(node->children[1]);
		}
	}

	void InsertNode(AABBTreeNode* node, AABBTreeNode** parent)
	{
		AABBTreeNode* p = *parent;
		if (p->IsLeaf())
		{
			// parent is leaf, simply split
			AABBTreeNode* newParent = new AABBTreeNode();
			newParent->parent = p->parent;
			newParent->SetBranch(node, p);
			*parent = newParent;
		}
		else
		{
			// parent is branch, compute volume differences 
			// between pre-insert and post-insert
			const AABB& aabb0 = p->children[0]->aabbFat;
			const AABB& aabb1 = p->children[1]->aabbFat;
			const double volumeDiff0 =
				Union(aabb0, node->aabbFat).Area() - aabb0.Area();
			const double volumeDiff1 =
				Union(aabb1, node->aabbFat).Area() - aabb1.Area();

			// insert to the child that gives less volume increase
			if (volumeDiff0 < volumeDiff1)
				InsertNode(node, &p->children[0]);
			else
				InsertNode(node, &p->children[1]);
		}

		// update parent AABB
		// (propagates back up the recursion stack)
		(*parent)->UpdateAABB();
	}

	void RemoveNode(AABBTreeNode* node)
	{
		// replace parent with sibling, remove parent node
		AABBTreeNode* parent = node->parent;
		if (parent) // node is not root
		{
			AABBTreeNode* sibling = node->GetSibling();
			if (parent->parent) // if there's a grandparent
			{
				// update links
				sibling->parent = parent->parent;
				(parent == parent->parent->children[0]
					? parent->parent->children[0]
					: parent->parent->children[1]) = sibling;
			}
			else // no grandparent
			{
				// make sibling root
				AABBTreeNode* sibling = node->GetSibling();
				m_root = sibling;
				sibling->parent = nullptr;
			}
			delete node;
			delete parent;
		}
		else // node is root
		{
			m_root = nullptr;
			delete node;
		}
	}

	void ClearChildrenCrossFlagHelper(AABBTreeNode* node)
	{
		node->childrenCrossed = false;
		if (!node->IsLeaf())
		{
			ClearChildrenCrossFlagHelper(node->children[0]);
			ClearChildrenCrossFlagHelper(node->children[1]);
		}
	}

	void CrossChildren(AABBTreeNode* node)
	{
		if (!node->childrenCrossed)
		{
			ComputePairsHelper(node->children[0],
				node->children[1]);
			node->childrenCrossed = true;
		}
	}

	void ComputePairsHelper(AABBTreeNode* n0, AABBTreeNode* n1)
	{
		if (n0->IsLeaf())
		{
			// 2 leaves, check proxies instead of fat AABBs
			if (n1->IsLeaf())
			{
				if (Collides(*n0->data, *n1->data))
					m_pairs.push_back(AllocatePair(n0->data, n1->data));
			}
			// 1 branch / 1 leaf, 2 cross checks
			else
			{
				CrossChildren(n1);
				ComputePairsHelper(n0, n1->children[0]);
				ComputePairsHelper(n0, n1->children[1]);
			}
		}
		else
		{
			// 1 branch / 1 leaf, 2 cross checks
			if (n1->IsLeaf())
			{
				CrossChildren(n0);
				ComputePairsHelper(n0->children[0], n1);
				ComputePairsHelper(n0->children[1], n1);
			}
			// 2 branches, 4 cross checks
			else
			{
				CrossChildren(n0);
				CrossChildren(n1);
				ComputePairsHelper(n0->children[0], n1->children[0]);
				ComputePairsHelper(n0->children[0], n1->children[1]);
				ComputePairsHelper(n0->children[1], n1->children[0]);
				ComputePairsHelper(n0->children[1], n1->children[1]);
			}
		} // end of if (n0->IsLeaf())
	}
};

/*
struct Position;
struct BaseObject;
struct AABB;
struct AABBTreeNode;
typedef pair<AABB, AABB> ColliderPair;
typedef vector<ColliderPair> ColliderPairList;
*/

/*	Position pos;
	int shapeType;//几何形状的种类：0为圆形，1为矩形
	double radius;//圆形半径
	double width;//矩形宽度
	double height;//矩形高度*/
