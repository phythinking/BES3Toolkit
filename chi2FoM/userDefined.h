#include <string>
#include <iostream>

std::string truncatTailingZeroes(std::string& s)
{
	//删除尾部多余的0，如果尾部以点结束，也删除小数点
	std::string tmps = s;
	if(tmps.find(".") != std::string::npos)
	{
		for (int i = tmps.size() - 1; i > 0; i--)
		{
//			std::cout << "tmps: " << tmps << "\n";
			if(tmps.at(i) == '0' || tmps.at(i) == '.') 
				tmps.erase(i);
			else
				break;
		}
//		std::cout << "tmps: " << tmps << "\n";
		return tmps;
	}
//	std::cout << "tmps: " << tmps << "\n";
	return tmps;
}

//int main()
//{
//	std::string s = std::to_string(2.000);
////	std::string s = std::to_string(2.03450);
//	std::string news = truncatTailingZeroes(s);
//	std::cout << s << " change to " << news << "\n";
//
//	return 0;
//}
