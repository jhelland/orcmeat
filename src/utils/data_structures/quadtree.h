// Created by jhelland (1/13/19)
//


#ifndef __QUADTREE_H__
#define __QUADTREE_H__


#include <vector>
#include <unordered_set>

#include <SFML/Graphics.hpp>


template<typename T, typename U>
class Quadtree {
private:
	typedef Quadtree<T, U> Node;
	typedef sf::Rect<U> Rect;

	const unsigned maxObjects = 10;
	const unsigned maxDepth = 5;

	std::vector<std::pair<T, Rect>> objects;
	std::vector<std::unique_ptr<Node>> children;
	int depth;
	Rect bounds;	

private:
	void split() {  // Split node into 4
		U subWidth = bounds.width / (U)2;
		U subHeight = bounds.height / (U)2;
		U x = bounds.left;
		U y = bounds.top;

		children[0] = std::move(std::make_unique<Node>(depth + 1, Rect(x + subWidth, y, subWidth, subHeight)));				// quadrant I
		children[1] = std::move(std::make_unique<Node>(depth + 1, Rect(x, y, subWidth, subHeight)));							// quadrant II
		children[2] = std::move(std::make_unique<Node>(depth + 1, Rect(x, y + subHeight, subWidth, subHeight)));				// quadrant III
		children[3] = std::move(std::make_unique<Node>(depth + 1, Rect(x + subWidth, y + subHeight, subWidth, subHeight)));	// quadrant IV
	}


	// Utility function to determine which quadrant a rectangle should belong to
	int get_index(const Rect& rect) {
		int idx = -1;  // -1 means belongs to parent
		U vertMidpoint = bounds.left + bounds.width / (U)2;
		U horizMidpoint = bounds.top + bounds.height / (U)2;

		// Rectangle fits entirely in top quadrants
		bool topQuad = rect.top < horizMidpoint && rect.top + rect.height < horizMidpoint;

		// Rectangle fits entirely in bottom quadrants
		bool botQuad = rect.top > horizMidpoint;

		// Rectangle fits entirely in left quadrants
		if (rect.left < vertMidpoint && rect.left + rect.width < vertMidpoint) {
			if (topQuad)
				idx = 1;
			else if (botQuad)
				idx = 2;
		}

		// Rectangle fits entirely in right quadrants
		if (rect.left > vertMidpoint) {
			if (topQuad)
				idx = 0;
			else if (botQuad)
				idx = 3;
		}

		return idx;
	}

public:
	Quadtree(int depth, Rect bounds) {
		this->depth = depth;
		this->bounds = bounds;

		children.resize(4);
		for (auto& child : children)
			child = nullptr;
	}


	//~Quadtree() { clear(); }


	void clear() {
		objects.clear();

		for (auto&& child : children) {
			if (child != nullptr) {
				child->clear();
				child.reset(nullptr);
			}
		}
	}


	void insert(std::pair<T, Rect> obj) {
		if (children[0] != nullptr) {
			int idx = get_index(obj.second);
			if (idx != -1) {
				children[idx]->insert(obj);
				return;
			}
		}

		objects.push_back(obj);

		if (objects.size() > maxObjects && depth < maxDepth) {
			if (children[0] == nullptr)
				split();

			int i = 0;
			while (i < objects.size()) {
				int idx = get_index(objects[i].second);
				if (idx != -1) {
					children[idx]->insert(objects[i]);
					objects.erase(objects.begin() + i);
				}
				else
					++i;
			}
		}
	}


	// Returns all objects within collision-checking range for target object
	void retrieve(std::unordered_set<T>& retObjs, const Rect& rect) {
		int idx = get_index(rect);
		if (idx != -1 && children[0] != nullptr)
			children[idx]->retrieve(retObjs, rect);

		// Merge set of IDs
		for (auto& it : objects)
			retObjs.insert(it.first);
	}
};


#endif