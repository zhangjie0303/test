/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class HelloWorldJNI */

#ifndef _Included_HelloWorldJNI
#define _Included_HelloWorldJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     HelloWorldJNI
 * Method:    sayHello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_HelloWorldJNI_sayHello
  (JNIEnv *, jobject);

/*
 * Class:     HelloWorldJNI
 * Method:    sumIntegers
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_HelloWorldJNI_sumIntegers
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     HelloWorldJNI
 * Method:    itoa
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_HelloWorldJNI_itoa
  (JNIEnv *, jobject, jlong);

/*
 * Class:     HelloWorldJNI
 * Method:    createUser
 * Signature: (Ljava/lang/String;)LUserData;
 */
JNIEXPORT jobject JNICALL Java_HelloWorldJNI_createUser
  (JNIEnv *, jobject, jstring);

/*
 * Class:     HelloWorldJNI
 * Method:    printUserName
 * Signature: (LUserData;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_HelloWorldJNI_printUserName
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
