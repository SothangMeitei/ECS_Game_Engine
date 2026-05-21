#pragma once
#include<string>

struct cTextOutput {
	std::string text;
	cTextOutput() = default;
	cTextOutput(const std::string& inputText) : text{ inputText } {}
};