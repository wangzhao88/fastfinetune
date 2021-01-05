#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include "stdafx.h"

using namespace std;

#define ROW 4
#define COL 3 + 1

class DetectionTree
{
public:
	struct TreeNode
	{
		string attr;							/* 当前的结点针对的是什么特征 */
		vector<TreeNode *>children;				/* 子节点 */
		map<string, TreeNode*> Attr2children;	/* 特征的每个属性对应的子节点 */
	};

	struct attribute
	{
		int index;								/* 属性的序列 */
		int total_num;							/* 每个属性的可选个数 */
		vector<string> attr_name;				/* 属性的每个可选名字 */
	};

	TreeNode *root;
	vector<attribute> attr;
	TreeNode* ID3(vector<vector<string>> data, vector<attribute> attr);
	void convert_data_to_attr(vector<vector<string>> data, vector<attribute> &attr);
};