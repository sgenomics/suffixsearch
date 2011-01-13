#include "URIStore.h"
//StringStore<template<class T> class object_store_type=ObjectStoreMem> BinaryNode<object_store_type>::s_uri_string_store = StringStore<object_store_type>();

//template<template<class T> class object_store_type> StringStore<object_store_type> BinaryNode<object_store_type>::s_uri_string_store = StringStore<object_store_type>();
StringStore BinaryNode::s_uri_string_store = StringStore();

