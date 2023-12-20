#include "dependencies.cpp"

class gPages
{
public:
	int countOfItems;

	vector<string> nameItems;
	vector<string> urlImageItems;
	vector<int> priceItems;
	vector<int> countOfVout;
	
	void getPages()
	{
		int count = getCountOfPages();

		int price, res = 0;
		string st, name, path;
		ifstream ifs;

		const char* applicationPath = "net\\GetPages.exe";
		const char* command = applicationPath;

		res = system(applicationPath);

		if (res != 1)
		{
			system("cls");
			for (int i = 1; i <= count; i++)
			{
				path = "pages/page" + to_string(i) + ".html";
				ifs.open(path);
				do
				{
					getline(ifs, st);
					if (st.find("\"item_def_icon\"") < st.size())
					{
						urlImageItems.push_back(getUrlImage(st));
						fsGetLine(ifs, st, 4);
						nameItems.push_back(getName(st));
						fsGetLine(ifs, st, 2);
						priceItems.push_back(toNumber(st));
						countOfVout.push_back(0);
					}
				} while (!ifs.eof());
				ifs.close();
			}

			for (int i = 0; i < nameItems.size(); i++)
			{
				cout << "Number: " << i << "\nName: " << nameItems[i] << "\nPrice: " << priceItems[i] << "\nCount of vout: " << countOfVout[i] << "\nurlImage: " << urlImageItems[i] << endl << endl;
			}
			countOfItems = nameItems.size();
			countOfVouts();
		}
		else cout << "��� �������";
	}

	void fsGetLine(ifstream &ifs, string &st, int count)
	{
		for(int i = 0; i < count; i++)
		{
			getline(ifs, st);
		}
	}

	string userVout(string key, int number, vector<int> countOfVout, int countOfItems)
	{
		ifstream ifs;
		string st = "";
		ifs.open("users/" + key + ".txt");
		if (!ifs.is_open())
		{
			ifs.close();
			ofstream ofs;
			ofs.open("users/" + key + ".txt");
			for (int i = 0; i < countOfItems; i++)
			{
				st += '-';
			}
			st[number] = '+';
			ofs << st;
			changeCountOfVouts('+', number, countOfVout, countOfItems);
			ofs.close();
			return "+";
		}
		else
		{
			if (ifs.peek() == EOF)
			{
				ifs.close();
				ofstream ofs;
				ofs.open("users/" + key + ".txt");
				for (int i = 0; i < countOfItems; i++)
				{
					st += '-';
				}
				st[number] = '+';
				ofs << st;
				changeCountOfVouts('+', number, countOfVout, countOfItems);
				ofs.close();
				return "+";
			}
			else
			{
				getline(ifs, st);
				ifs.close();
				ofstream ofs;
				ofs.open("users/" + key + ".txt", ios::out);
				if (st[number] == '+')
				{
					st[number] = '-';
					changeCountOfVouts('-', number, countOfVout, countOfItems);
					ofs << st;
					ofs.close();
					return "-";
				}
				else
				{
					changeCountOfVouts('+', number, countOfVout, countOfItems);
					st[number] = '+';
					ofs << st;
					ofs.close();
					return "+";
				}
			}
		}
	}

	void countOfVouts()
	{
		ifstream ifs;
		ifs.open("countOfVouts.txt");
		if (!ifs.is_open())
		{
			ifs.close();
			ofstream ofs;
			ofs.open("countOfVouts.txt");
			for (int i = 0; i < countOfVout.size(); i++)
			{
				ofs << countOfVout[i] << "\n";
			}
			ofs.close();
		}
		else
		{
			if (ifs.peek() == EOF)
			{
				ifs.close();
				ofstream ofs;
				ofs.open("countOfVouts.txt");
				for (int i = 0; i < countOfVout.size(); i++)
				{
					ofs << countOfVout[i] << "\n";
				}
				ofs.close();
			}
			else
			{
				int i = 0, h;
				string st;
				while (i < countOfItems)
				{
					getline(ifs, st);
					countOfVout[i] = atoi(st.c_str());
					i++;
				}
				ifs.close();
			}
		}
	}

	void changeCountOfVouts(char h, int number, vector<int> countOfVout, int countOfItems)
	{
		string st;
		ifstream ifs;
		int n;
		ifs.open("countOfVouts.txt");
		for (int i = 0; i < number + 1; i++)
		{
			getline(ifs, st);
		}
		n = atoi(st.c_str());
		ifs.close();
		ofstream ofs;
		ofs.open("countOfVouts.txt", ios::out);
		if (h == '+')
		{
			n = n + 1;
			countOfVout[number] = n;
			for (int i = 0; i < countOfItems; i++)
			{
				ofs << countOfVout[i] << "\n";
			}
		}
		else
		{
			n = n - 1;
			countOfVout[number] = n;
			for (int i = 0; i < countOfItems; i++)
			{
				ofs << countOfVout[i] << "\n";
			}
		}
		ofs.close();
	}

	static size_t write_data(char* ptr, size_t size, size_t nmemb, string* data)
	{
		if (data)
		{
			data->append(ptr, size * nmemb);
			return size * nmemb;
		}
		else
			return 0;
	}

	string getUrlImage(string st)
	{
		int f;
		f = st.find("src");
		st.erase(0, f);
		f = st.find("\"");
		st.erase(0, f+1);
		f = st.find("\"");
		st.erase(f, st.size());
		return st;
	}

	int toNumber(string st)
	{
		string h;
		for (int i = 0; i < st.size(); i++)
		{
			if (st[i] >= 48 && st[i] <= 57)
			{
				h += st[i];
			}
		}
		int res = atoi(h.c_str());
		return res;
	}

	string getName(string st)
	{
		int h;
		h = st.find("</a>");
		st.erase(h, st.size() - h);
		h = st.rfind(">");
		st.erase(0, h + 1);
		return st;
	}

	string createPersonalCode()
	{
		int sizeC = 80;
		char ch;
		string st;
		for (int i = 0; i < sizeC; i++)
		{
			int h = GetRandomNumber(0, 1);
			if (h == 0)
			{
				ch = GetRandomNumber(65, 90);
				st += ch;
			}
			else
			{
				ch = GetRandomNumber(48, 57);
				st += ch;
			}
		}
		ofstream ofs;
		ofs.open("userKeys.txt", ios::app);
		ofs << st << "\n";
		ofs.close();
		return st;
	}

	int GetRandomNumber(int min, int max)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(min, max);
		int randomValue = dist(mt);
		return randomValue;
	}

	int getCountOfPages()
	{
		string content;
		int count = 0, h = 0;

		CURL* curl_handle;
		curl_handle = curl_easy_init();

		if (curl_handle)
		{
			curl_easy_setopt(curl_handle, CURLOPT_URL, "https://store.steampowered.com/itemstore/252490/browse/?filter=All");

			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);

			CURLcode res = curl_easy_perform(curl_handle);

			curl_easy_cleanup(curl_handle);
		}

		ofstream ofs;
		ofs.open("coutnOfPages.txt");
		ofs << content;
		ofs.close();

		ifstream ifs("coutnOfPages.txt");
		do
		{
			getline(ifs, content);
			if (content.find("ItemDefs_total") < content.size())
			{
				content.erase(0, content.find("ItemDefs_total"));
				break;
			}
		} while (!ifs.eof());
		ifs.close();

		count = toNumber(content);
		do
		{
			if (count > 0)
			{
				h++;
				count = count - 12;
			}
			else break;
		} while (true);

		ofs.open("coutnOfPages.txt");
		ofs << h;
		ofs.close();

		return h;
	}
};