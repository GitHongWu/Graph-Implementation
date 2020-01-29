/*minqueue.h*/

//
// Min queue that stores (key, value) pairs using a min-heap
// implementation.  When pop is called, the key from the
// (key, value) pair with the smallest value is returned; if
// two pairs have the same value, the smaller key is returned.
// Push and pop have O(lgN) time complexity.
//
// << Hongcheng Wu >>
//
// Original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #07
//

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <map>

using namespace std;

template <typename TKey, typename TValue>
class minqueue
{
private:
	struct NODE
	{
		TKey    Key;
		TValue  Value;
	};

	NODE *Queue;	//dynamic array
	int Capacity;
	int CurrentQueueSize;
	map<TKey, int> mb;	//map key is TKey, value is index

public:
	//
	// default constructor:
	//
	// Queue has a max capacity for efficient implementation.
	// This max capacity must be specified at queue creation.
	//
	minqueue(int capacity)
	{
		Queue = new NODE[capacity];
		Capacity = capacity;
		CurrentQueueSize = 0;
	}

	//
	// fill constructor:
	//
	// This allows for the efficient O(N) construction of
	// a queue with an initial set of keys, all with the same
	// initial value.  The max capacity of the queue is
	// set to the # of keys provided for initialization;
	// it is assumed the keys are in ascending order.
	//
	minqueue(vector<TKey> keys, TValue initialValue)
	{
		// if(keys.size() > Size){
		// 	throw runtime_error("Given vector is too large");
		// }
		Capacity = keys.size();	//set Capacity
		CurrentQueueSize = Capacity;
		Queue = new NODE[Capacity]; //allocate array
		NODE temp;
		for (int i = 0; i < keys.size(); i++)
		{
			temp.Key = keys[i];
			temp.Value = initialValue;
			Queue[i] = temp;
			mb[temp.Key] = i;
		}
	}

	//
	// destructor:
	//
	virtual ~minqueue()
	{
		//
		// TODO:
		//
		delete[] Queue;
		Capacity = 0;
		CurrentQueueSize = 0;
	}

	//
	// empty:
	//
	// Returns true if empty, false if not.
	//
	bool empty()
	{
		if(CurrentQueueSize == 0){
			return true;
		}
		return false;
	}
	
	//swap by given two indexes
	void swap(int nodeIndex, int parentIndex){
		//reset map
		mb.erase(Queue[nodeIndex].Key);
		mb.erase(Queue[parentIndex].Key);
		mb[Queue[nodeIndex].Key] = parentIndex;
		mb[Queue[parentIndex].Key] = nodeIndex;

		NODE temp;
		temp = Queue[nodeIndex];
		Queue[nodeIndex] = Queue[parentIndex];
		Queue[parentIndex] = temp;
	}

	//Percolate Up with given value and index
	void heapPercolateUp(int nodeIndex) {
		int parentIndex;
		while (nodeIndex > 0)
		{
			parentIndex = (nodeIndex - 1) / 2;
			if (Queue[nodeIndex].Value == Queue[parentIndex].Value){	//value is equal
				if(Queue[nodeIndex].Key < Queue[parentIndex].Key){	//order by key
					swap(nodeIndex, parentIndex);
					mb[Queue[parentIndex].Key] = parentIndex;	//add to map
					return;
				}
				mb[Queue[nodeIndex].Key] = nodeIndex;
				return; //if key is ordered, then just return
			}
			else if (Queue[nodeIndex].Value < Queue[parentIndex].Value){
				swap(nodeIndex, parentIndex);
				nodeIndex = parentIndex;
			}
			else
			{
				mb[Queue[nodeIndex].Key] = nodeIndex;
				return;
			}
		}
	}

	//Percolate Down 
	void heapPercolateDown(int nodeIndex){

		while(nodeIndex < CurrentQueueSize){
			int childLeftIndex = 2 * nodeIndex + 1;
			int childRightIndex = 2 * nodeIndex + 2;
		
			if (CurrentQueueSize <= childLeftIndex && CurrentQueueSize <= childRightIndex)	//no children
			{
				mb[Queue[nodeIndex].Key] = nodeIndex;
				return;
			}
			else if(CurrentQueueSize <= childRightIndex)	//only have left child
			{
				swap(nodeIndex, childLeftIndex);
				nodeIndex = childLeftIndex;
			}
			else{
				if(Queue[childLeftIndex].Value <= Queue[childRightIndex].Value){
					swap(nodeIndex, childLeftIndex);
					nodeIndex = childLeftIndex;
				}
				else{
					swap(nodeIndex, childRightIndex);
					nodeIndex = childRightIndex;
				}
			}
		}
	}

	//
	// push:
	//
	// Inserts the given (key, value) pair into the queue such that
	// pop always returns the pair with the minimum value.  If the
	// key is *already* in the queue, it's value is updated to the
	// given value and the queue reordered.  If the key is not in
	// the queue, the (key, value) pairs is added and the queue
	// reordered.
	//
	// NOTE: if two keys have the same value, i.e. (key1, value) and
	// (key2, value), then those pairs are ordered into ascending value
	// by their key.
	//
	void pushinorder(TKey key, TValue value)
	{
		//if key exist update value
		if (mb.find(key) != mb.end())
		{
			int index = mb.at(key);
			Queue[index].Value = value;	//update value
			if(value > Queue[2 * index + 1].Value
				|| value > Queue[2 * index + 2].Value)	//value larger than any of children
			{
				heapPercolateDown(index);
			}
			else if(value <= Queue[(index - 1) / 2].Value){	//value smaller than it's parent
				heapPercolateUp(index);
			}
			return;
		}
		
		//
		// we need to insert a new (key, value) pair but the queue is full:
		//
		if (CurrentQueueSize == Capacity)
		{
			throw runtime_error("minqueue::pushinorder: queue full");
		}

		NODE temp;
		temp.Key = key;
		temp.Value = value;
		Queue[CurrentQueueSize] = temp;
		heapPercolateUp(CurrentQueueSize);
		CurrentQueueSize++;
	}

	//
	// front:
	//
	// Returns the key at the front of the queue; does *not* pop the
	// (key, value) pair.  Throws a logic_error exception if the queue
	// is empty.
	//
	TKey minfront()
	{
		if (empty())
		{
			throw logic_error("minqueue::minfront: queue empty");
		}
		return Queue[0].Key;
	}

	//
	// pop:
	//
	// Pops and discards the (key, value) pair at the front of the queue.
	// Throws a logic_error exception if the queue is empty.
	//
	void minpop()
	{
		if (empty())
		{
			throw logic_error("minqueue::minpop: queue empty");
		}

		mb.erase(Queue[0].Key);
		Queue[0] = Queue[CurrentQueueSize - 1];
		CurrentQueueSize--;
		heapPercolateDown(0);
		
	}

	void outputQueue(){
		int counter = 0;
		while (counter < CurrentQueueSize){
			cout << "Key: " << Queue[counter].Key << " Value: " << Queue[counter].Value << endl;
			counter++;
		}
	}
};