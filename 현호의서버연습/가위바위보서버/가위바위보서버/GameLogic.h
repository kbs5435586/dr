#pragma once

void SetGameTitle(char* _szTitle);

void Render_Title(int count = 6);
char InputItem_Client();
char InputItem_Server();

bool Render_Result(char _itemClient, char _itemServer, char _result);
