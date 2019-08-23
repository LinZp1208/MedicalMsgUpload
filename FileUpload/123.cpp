//POST json
int main()
{
	CURL *curl;
	CURLcode res;
	char szJsonData[1024];
	//HTTP报文头  
	struct curl_slist* headers = NULL;

	char *url = "http://if.qdocument.net:705/bic/operationNote/upload";

	curl = curl_easy_init();

	if (curl)
	{
		//string类型的json串
		memset(szJsonData, 0, sizeof(szJsonData));
		std::string strJson = "{";
		strJson += "\"uid\" : \"chechenluoyang@163.com\",";
		strJson += "\"fileName\" : \"梅西.txt\",";
		strJson += "\"time\" : \"2017.07.28 10:55:22\",";
		strJson += "\"type\" : \"Libcurl HTTP POST JSON \",";
		strJson += "\"authList\" : \"123\"";
		strJson += "}";
		strcpy(szJsonData, strJson.c_str());

		strJson = string AsciiToUtf8(strJson);//如果json串中包含有中文,必须进行转码

		std::wstring wstrJson = _T("{");
		wstrJson += _T("\"uid\" : \"chechenluoyang@163.com\",");
		wstrJson += _T("\"fileName\" : \"梅西.txt\",");
		wstrJson += _T("\"time\" : \"2017.07.29 10:55:22\",");
		wstrJson += _T("\"type\" : \"Libcurl HTTP POST JSON \",");
		wstrJson += _T("\"authList\" : \"test\"");
		wstrJson += _T("}");

		string  testJson = UnicodeToUtf8(wstrJson);

		std::stringstream out;

		//设置url
		curl_easy_setopt(curl, CURLOPT_URL, url);
		// 设置http发送的内容类型为JSON

		//构建HTTP报文头  

		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");



		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, testJson.c_str());//以Unicode编码上传
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数



		res = curl_easy_perform(curl);


		curl_easy_cleanup(curl);
	}
	return 0;
}