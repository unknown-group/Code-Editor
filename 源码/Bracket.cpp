/*
杜科松0
括号等自动补全
每次输入一个字符调用一次这个函数
返回对应的补全，如果是0就不补全
*/
char Bracket_completion(char c)
{
	if (c == '{')
	{
		return '}'; 
	}
	else if (c == '(')
	{
		return ')';
	}
	else if (c == '[') 
	{
		return ']';
	}
	else if (c == '<')
	{
		return '>';
	}
	else if (c == '"')
	{
		return '"';
	}
	else
	{
		return 0;
	}
}
/*
杜科松
每次光标移动后如果在左括号左边或者是右括号右边就调用一次该函数
返回一个数，表示匹配的符号距离光标所指的符号的距离，正数代表在右边，负数代表在左边，0表示没有匹配的符号
*/
int Bracket_matching(string str)
{

}
