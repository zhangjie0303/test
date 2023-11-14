#include <stdio.h>
#include <iostream>
#include "HelloWorldJNI.h"
 
#if 1
jstring toJavaString(JNIEnv* env, const std::string* string,
    const bool treat_empty_as_null = false) {
  if (string == nullptr) {
    return nullptr;
  }

  if (treat_empty_as_null && string->empty()) {
    return nullptr;
  }

  return env->NewStringUTF(string->c_str());
}
#endif

// java call cpp
JNIEXPORT void JNICALL Java_HelloWorldJNI_sayHello
  (JNIEnv* env, jobject thisObject) {
    printf("hello world!\n");
}

JNIEXPORT jlong JNICALL Java_HelloWorldJNI_sumIntegers
  (JNIEnv *env, jobject thisObject, jint first, jint second) {
    printf("received first:%d second:%d\n", first, second);
    long sum = (long)first + (long)second;
    return sum;
}

JNIEXPORT jstring JNICALL Java_HelloWorldJNI_itoa
  (JNIEnv *env, jobject thisObject, jlong num) {
    std::string ret = std::to_string(num);
    return toJavaString(env, &ret);
    //return (*env)->NewStringUTF("zhangjie");
}

#if 1
// cpp call java
JNIEXPORT jobject JNICALL Java_HelloWorldJNI_createUser
  (JNIEnv *env, jobject thisObject, jstring myName){
    // 创建 UserData 类对象
    jclass userDataClass = env->FindClass("UserData");
    jobject newUserData = env->AllocObject(userDataClass);
 
    // 获取需要UserData类的name成员字段
    jfieldID nameField = env->GetFieldID(userDataClass , "name", "Ljava/lang/String;");
 
    // 给UserData类的name成员赋值
    env->SetObjectField(newUserData, nameField, myName);
    
    // 返回创建的对象
    return newUserData;
}
 
JNIEXPORT jstring JNICALL Java_HelloWorldJNI_printUserName
  (JNIEnv *env, jobject thisObject, jobject userData){
    // 获取createUser创建的userData类
    jclass userDataClass = env->GetObjectClass(userData);
    
    // 获取getUserName方法id
    jmethodID methodId = env->GetMethodID(userDataClass, "getUserName", "()Ljava/lang/String;");
 
    // 调用getUserName方法，获取返回值，保存到result
    jstring result = (jstring)env->CallObjectMethod(userData, methodId);
    
    // 打印getUserName获取的返回值
    printf("My name is: %s, from CPP.\n", env->GetStringUTFChars(result, NULL));
    
    return result;
}
#endif
