#include"DetectionTree.h"

void get_new_table(vector<vector<string>> ori_data, vector<vector<string>> &data, int min_index, string name)
{
	int row = ori_data.size();
	int col = ori_data.size();

	for (int i = 0; i < row; ++i)
	{
		if (ori_data[i][min_index] == name)
		{

			vector<string> tem;
			for (int j = 0; j < col; ++j)
			{
				if (j != min_index)
				{
					tem.push_back(ori_data[i][j]);
				}
			}

			data.push_back(tem);
		}
	}


}

void DetectionTree::convert_data_to_attr(vector<vector<string>> data, vector<attribute> &attr)
{
	for (int i = 0; i < data[0].size() - 1; ++i)
	{
		attribute tem;
		set<string> name;
		vector<string> str_name;

		for (int j = 0; j < data.size(); ++j)
		{
			name.insert(data[j][i]);
		}

		tem.total_num = name.size();
		tem.index = i;

		for (auto x : name)
		{
			str_name.push_back(x);
		}

		tem.attr_name = str_name;

		attr.push_back(tem);
	}
}

/* 计算当前数据集的信息熵 */
float calculate_wholeHD(vector<vector<string>> data)
{
	 if(data.size() == 0)
	{
		return -1;
	}

	int row = data.size();
	int col = data[0].size();
	int pos_num = 0;
	int neg_num = 0;

	for (int i = 0; i < row; ++i)
	{
		if (data[i][col - 1] == "1")
		{
			pos_num++;
		}
		else
		{
			neg_num++;
		}
	}

	return ((float)pos_num / row) * log((float)pos_num / row) + ((float)neg_num / row) * log((float)neg_num / row);
}

/* 计算某个特征下的条件熵 */
float calculate_condiationHD(vector<vector<string>> data, int index)
{
	if (data.size() == 0)
	{
		return 0.0;
	}
		
	map<string, int> attr;
	int row = data.size();
	int col = data[0].size();

	int attr_num = 0;
	for (int i = 0; i < row; ++i)
	{
		if (attr.find(data[i][index]) == attr.end())
		{
			attr.insert(make_pair(data[i][index], attr_num));
			attr_num++;
		}
	}

	vector<int> pos_num(attr_num);
	vector<int> neg_num(attr_num);

	for (int i = 0; i < row; ++i)
	{
		int num = attr[data[i][index]];
		if (data[i][col - 1] == "1")
		{
			pos_num[num]++;
		}
		else
		{
			neg_num[num]++;
		}
	}

	float res = 0.0;
	for (int i = 0; i < attr_num; ++i)
	{
		int total = pos_num[i] + neg_num[i];
		res = res + ((float)pos_num[i] / total) * (pos_num[i] == 0? 0:-log((float)pos_num[i] / total)) + 
			((float)neg_num[i] / total) * (neg_num[i] == 0? 0:-log((float)neg_num[i] / total));
	}


	return res;
}

/* ID3算法 */
DetectionTree::TreeNode* DetectionTree::ID3(vector<vector<string>> data, vector<attribute> attr)
{
	TreeNode* resNode = new TreeNode;

	if (data.size() == 0 || attr.size() == 1)
	{
		return NULL;
	}

	if (data[0].size() == 1)
	{
		return NULL;
	}

	int wholeHD = calculate_wholeHD(data);

	int candidate_num = attr.size();
	int min_index = 0;
	float minHD = 2.0;

	/* 选取信息增益最大的属性，最后一列是标志不考虑 */
	for (int i = 0; i < candidate_num; ++i)
	{
		int HD = calculate_condiationHD(data, i);
		if(HD < minHD)
		{
			min_index = i;
			minHD = HD;
		}
	}

	if (minHD == 2.0)
	{
		return NULL;
	}

	/* 分裂子树 */
	vector<string> attr_tem = attr[min_index].attr_name;
	attr.erase(attr.begin() + min_index);
	for (int i = 0; i < attr_tem.size(); ++i)
	{
		TreeNode* childNode = new TreeNode;
		vector<vector<string>> tempMat;
		string name = attr_tem[i];

		get_new_table(data, tempMat, min_index, name);
		childNode = this->ID3(tempMat, attr);

		resNode->attr = to_string(min_index);
		resNode->children.push_back(childNode);
		resNode->Attr2children.insert(pair<string, TreeNode*>(name, childNode));
	}

	return resNode;
}



