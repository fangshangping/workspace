#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "../3rd/mjson/json.h"
/*
{
	"uid":123,
	"uname":"hello!",
	"is_new":true
	"vip":null,

	"man_prop":[1,"hello","2"],
	"weap_prop":{
	"default":"putongzidan",
	},
}

 
*/

static char json_str[4096];
 
int main(int argc,char ** argv)
{
	json_t* root = json_new_object();
	json_t* number = json_new_number("123");
	json_insert_pair_into_object(root, "uid", number);

	json_t* str = json_new_string("hello!");
	json_insert_pair_into_object(root, "uname", str);

	json_t* b_true = json_new_true();
	json_insert_pair_into_object(root, "is_new", b_true);

	json_t* j_null = json_new_null();
	json_insert_pair_into_object(root, "vip", j_null);

	json_t* j_array = json_new_array();
	json_insert_pair_into_object(root, "man_prop", j_array);
	number = json_new_number("1");
	json_insert_child(j_array, number);
	str = json_new_string("hello");
	json_insert_child(j_array, str);
	str = json_new_string("2");
	json_insert_child(j_array, str);

	json_t* j_object = json_new_object();
	json_insert_pair_into_object(root, "weap_prop", j_object);
	str = json_new_string("putongzidan");
	json_insert_pair_into_object(j_object, "default", str);

	char* json_text;
	json_tree_to_string(root, &json_text); //这个函数 malloc json所需要的函数
	printf("%s\n", json_text);
	strcpy(json_str, json_text);
	free(json_text); // 需要销毁

	//销毁整个json树
	json_free_value(&root);
	root = NULL;

	json_parse_document(&root, json_str);

	json_t* key = json_find_first_label(root, "uname");
	if (key)
	{
		json_t* value = key->child;
		switch (value->type)
		{
		case JSON_STRING:
			printf("key: %s value: %s\n", key->text, value->text);
			break;
		}
	}

	key = json_find_first_label(root, "uid");
	if (key)
	{
		json_t* value = key->child;
		switch (value->type)
		{
		case JSON_NUMBER:
			printf("key: %s value: %d\n", key->text, atoi(value->text));
			break;
		}
	}

	system("pause");

	return 0; 
}