#include "tmx_reader.h"

using namespace std;

bool tage::init(string body_tage)
{
	body = "";
	tage_name = "";
	v_subtages.clear();
	characteristics.clear();

	if (body_tage.size() == 0)
		return false;

	char a;
	int count_frame = 0;
	for (int i(0); i < body_tage.size(); i++)
	{
		a = body_tage[i];
		if (a == '>' || a == '<')
			count_frame++;
	}
	if (count_frame < 2)
		return false;
	if (count_frame == 2)
	{
		string head = "";

		for (int i(1); i < body_tage.size() - 1; i++)
		{
			a = body_tage[i];
			head += a;
		}

		tage_name = "";
		bool ok = false;
		int t = 0;
		while (!ok && t < head.size())
		{
			a = head[t];
			if (a != ' ')
				tage_name += a;
			else
				ok = true;
			t++;
		}
		if (tage_name[tage_name.size() - 1] == '/' && tage_name.size() == head.size())
		{
			ok = true;
			tage_name = tage_name.substr(0, tage_name.size() - 1);
		}
		if (!ok)
		{
			return false;
		}

		string current_characteristic = "", current_value = "";
		bool before_eq = true, read_value = false;
		ok = false;
		while (t < head.size())
		{
			while (!ok && t < head.size())
			{
				a = head[t];
				if (a == '=')
				{
					if (a == '=' && before_eq)
					{
						before_eq = false;
						if (t == head.size())
							return false;
						a = head[t + 1];
						if (a != '\"')
							return false;
						t++;
					}
					else
						return false;
				}
				else
				{
					if (a == '\"' && !before_eq)
					{
						ok = true;
						t++;
					}
					else
					{
						if (before_eq)
							current_characteristic += a;
						else
							current_value += a;
					}
				}
				t++;
			}

			if (!before_eq)
			{
				characteristics[current_characteristic] = current_value;
				current_characteristic = current_value = "";
			}
			before_eq = true;
			ok = false;
		}
		return true;
	}
	else
	{
		bool ok;
		bool find_name;
		string name_first = "", name_last = "";
		int body_first = 0, body_last = body_tage.size() - 1;


		a = body_tage[0];
		ok = false;
		find_name = true;
		if (a != '<')
			return false;
		body_first++;
		for (int i(1); !ok && i < body_tage.size(); i++)
		{
			a = body_tage[i];
			if (a == '>')
				ok = true;
			else
			{
				if (a == ' ')
					find_name = false;
				if (find_name)
					name_first += a;
			}
			body_first++;
		}
		if (!ok)
			return false;
		a = body_tage[body_tage.size() - 1];
		ok = false;
		if (a != '>')
			return false;
		body_last--;
		for (int i(body_tage.size() - 2); !ok && i >= 0; i--)
		{
			a = body_tage[i];
			if (a == '<')
				ok = true;
			else
				if (a != '/')
					name_last = a + name_last;
			body_last--;
		}
		if (!ok)
			return false;

		if (name_first != name_last)
			return false;
		body = body_tage.substr(body_first, body_last - body_first + 1);

		string head = "";
		bool in_head = true;
		for (int i(1); in_head && i < body_tage.size(); i++)
		{
			a = body_tage[i];
			if (a == '>')
				in_head = false;

			if (in_head)
				head += a;
		}


		tage_name = "";
		ok = false;
		int t = 0;
		while (!ok && t < head.size())
		{
			a = head[t];
			if (a != ' ')
				tage_name += a;
			else
				ok = true;
			t++;
		}
		if (!ok && t != head.size())
			return false;
		string current_characteristic = "", current_value = "";
		bool before_eq = true, read_value = false;
		ok = false;
		while (t < head.size())
		{
			while (!ok && t < head.size())
			{
				a = head[t];
				if (a == '=')
				{
					if (a == '=' && before_eq)
					{
						before_eq = false;
						if (t == head.size())
							return false;
						a = head[t + 1];
						if (a != '\"')
							return false;
						t++;
					}
					else
						return false;
				}
				else
				{
					if (a == '\"' && !before_eq)
					{
						ok = true;
						t++;
					}
					else
					{
						if (before_eq)
							current_characteristic += a;
						else
							current_value += a;
					}
				}
				t++;
			}

			if (!before_eq)
			{
				characteristics[current_characteristic] = current_value;
				current_characteristic = current_value = "";
			}
			before_eq = true;
			ok = false;
		}


		string find_tage_name = "";
		string current_tage_name = "";
		string body_subtage = "";
		string current_string;
		vector <string> v_add_tags_texts;
		bool under = false;
		t = body.size() - 1;
		int k = body.size() - 1;
		while (t >= 0)
		{
			a = body[t];
			if (!under)
			{
				if (a == '>')
				{
					current_string = "";
					k = t - 1;
					t--;
					while (a != '<' && t >= 0)
					{
						a = body[t];
						if (a != '<')
							current_string = a + current_string;
						t--;
					}
					find_tage_name = "";
					body_subtage = "";
					a = current_string[0];
					if (a != '/')
					{
						k = 0;
						while (a != ' ' && k < current_string.size())
						{
							a = current_string[k];
							if (a != ' ')
								find_tage_name += a;
							k++;
						}
						v_add_tags_texts.push_back('<' + current_string + '>');
					}
					else
					{
						k = 1;
						while (a != ' ' && k < current_string.size())
						{
							a = current_string[k];
							if (a != ' ')
								find_tage_name += a;
							k++;
						}
						under = true;
						body_subtage = "<" + current_string + '>';
					}
				}
				else
				{
					t--;
				}
			}
			else
			{
				if (a == '>')
				{
					current_string = "";
					k = t - 1;
					t--;
					while (a != '<' && t >= 0)
					{
						a = body[t];
						if (a != '<')
							current_string = a + current_string;
						t--;
					}
					current_tage_name = "";
					a = current_string[0];
					k = 0;
					while (a != ' ' && k < current_string.size())
					{
						a = current_string[k];
						if (a != ' ')
							current_tage_name += a;
						k++;
					}
					if (current_tage_name == find_tage_name)
					{
						body_subtage = '<' + current_string + '>' + body_subtage;
						v_add_tags_texts.push_back(body_subtage);
						body_subtage = "";
						under = false;
					}
					else
					{
						body_subtage = '<' + current_string + '>' + body_subtage;
					}
				}
				else
				{
					body_subtage = a + body_subtage;
					t--;
				}
			}
		}


		tage add_tage;
		for (int i(v_add_tags_texts.size() - 1); i >= 0; i--)
		{
			if (!add_tage.init(v_add_tags_texts[i]))
				return false;
			v_subtages.push_back(add_tage);
		}
		return true;
	}
}
string tage::get_tage_name()
{
	return tage_name;
}
string tage::get_characteristic(string name_characteristic)
{
	if (characteristics.find(name_characteristic) != characteristics.end())
		return characteristics[name_characteristic];
	else
		return "";
}
string tage::get_body()
{
	return body;
}
vector <tage> tage::get_v_subtages()
{
	return v_subtages;
}
vector <tage> tage::get_v_subtages_with_name(string find_tage_name)
{
	vector <tage> result;
	result.clear();
	for (int i(0); i < v_subtages.size(); i++)
		if (v_subtages[i].get_tage_name() == find_tage_name)
			result.push_back(v_subtages[i]);
	return result;
}

bool tmx_file::load_from_file(string ADRESS)
{
	ifstream fin(ADRESS);
	if (!fin.is_open())
		return false;
	string text = "", add_str;
	string define_tage_str = "", main_tage_str = "";

	while (!fin.eof())
	{
		getline(fin, add_str);
		text += add_str;
	}

	if (text.size() == 0)
		return false;
	char a = text[0];
	int t = 0;
	while (a != '>' && t < text.size())
	{
		a = text[t];
		define_tage_str += a;
		t++;
	}
	while (t < text.size())
	{
		a = text[t];
		main_tage_str += a;
		t++;
	}

	tage define_tage;
	if (!define_tage.init(define_tage_str))
		return false;
	if (define_tage.get_tage_name() != "?xml")
		return false;
	version = define_tage.get_characteristic("version");
	encoding = define_tage.get_characteristic("encoding");
	
	string find_tage_name = "";
	string current_tage_name = "";
	string body_subtage = "";
	string current_string;
	vector <string> v_add_tags_texts;
	bool under = false;
	t = text.size() - 1;
	int k = text.size() - 1;
	while (t >= 0)
	{
		a = text[t];
		if (!under)
		{
			if (a == '>')
			{
				current_string = "";
				k = t - 1;
				t--;
				while (a != '<' && t >= 0)
				{
					a = text[t];
					if (a != '<')
						current_string = a + current_string;
					t--;
				}
				find_tage_name = "";
				body_subtage = "";
				a = current_string[0];
				if (a != '/')
				{
					k = 0;
					while (a != ' ' && k < current_string.size())
					{
						a = current_string[k];
						if (a != ' ')
							find_tage_name += a;
						k++;
					}
					v_add_tags_texts.push_back('<' + current_string + '>');
				}
				else
				{
					k = 1;
					while (a != ' ' && k < current_string.size())
					{
						a = current_string[k];
						if (a != ' ')
							find_tage_name += a;
						k++;
					}
					under = true;
					body_subtage = "<" + current_string + '>';
				}
			}
			else
			{
				t--;
			}
		}
		else
		{
			if (a == '>')
			{
				current_string = "";
				k = t - 1;
				t--;
				while (a != '<' && t >= 0)
				{
					a = text[t];
					if (a != '<')
						current_string = a + current_string;
					t--;
				}
				current_tage_name = "";
				a = current_string[0];
				k = 0;
				while (a != ' ' && k < current_string.size())
				{
					a = current_string[k];
					if (a != ' ')
						current_tage_name += a;
					k++;
				}
				if (current_tage_name == find_tage_name)
				{
					body_subtage = '<' + current_string + '>' + body_subtage;
					v_add_tags_texts.push_back(body_subtage);
					body_subtage = "";
					under = false;
				}
				else
				{
					body_subtage = '<' + current_string + '>' + body_subtage;
				}
			}
			else
			{
				body_subtage = a + body_subtage;
				t--;
			}
		}
	}


	tage add_tage;
	for (int i(v_add_tags_texts.size() - 1); i >= 0; i--)
	{
		if (!add_tage.init(v_add_tags_texts[i]))
			return false;
		if (add_tage.get_tage_name() != "?xml")
			v_general_tages.push_back(add_tage);
	}

	return true;
}
vector <tage> tmx_file::get_v_general_tages()
{
	return v_general_tages;
}
vector <tage> tmx_file::get_v_general_tages_with_tage_name(string find_name)
{
	vector <tage> result;
	result.clear();
	for (int i(0); i < v_general_tages.size(); i++)
		if (v_general_tages[i].get_tage_name() == find_name)
			result.push_back(v_general_tages[i]);
	return result;
}
string tmx_file::get_version()
{
	return version;
}
string tmx_file::get_encoding()
{
	return encoding;
}