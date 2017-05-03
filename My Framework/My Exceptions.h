#pragma once

#include <string>

class MyException
{
public:
	MyException( const wchar_t* fileName, unsigned int lineFound, const std::wstring& error = L"" )
		:
	fileName( fileName ),
	lineFound( lineFound ),
	error( error )
	{}
	const std::wstring& GetError() const
	{
		return error;
	}
	const std::wstring& GetFileName() const
	{
		return fileName;
	}
	const unsigned int GetLineFound() const
	{
		return lineFound;
	}
	std:: wstring GetLocation() const
	{
		return std::wstring( L"Line [" ) + std::to_wstring( lineFound ) + L"] in " + fileName;
	}
	virtual std::wstring GetErrorMessage() const = 0;
	virtual std::wstring GetExceptionType() const = 0;
private:
	std::wstring fileName;
	unsigned int lineFound;
	std::wstring error;
};