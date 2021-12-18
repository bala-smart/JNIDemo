#include <jni.h>        // JNI header provided by JDK
#include <iostream>
#include <string.h>
#include "JNIDemo.h"   // Generated
using namespace std;

// Implementation of the native method sayHello()
JNIEXPORT void JNICALL Java_jni_JNIDemo_sayHello(JNIEnv* env, jobject thisObj) {
	printf("Hello World!\n");
	return;
}

JNIEXPORT int JNICALL Java_jni_JNIDemo_getSum(JNIEnv* env, jobject thisObj,jint a, jint b) {
	printf("In C, the numbers are %d and %d\n", a, b);
	return a+b;
}

JNIEXPORT jobject JNICALL Java_jni_JNIDemo_getReverseString(JNIEnv *env, jobject thisObject, jstring JNIString) {
	const char* cString = env->GetStringUTFChars(JNIString, NULL);
	string reverseString;
	if (NULL == cString) {
		return NULL;
	}
	else {
		reverseString = _strrev((char *)cString);
		env->ReleaseStringUTFChars(JNIString, cString);
	}
	return env->NewStringUTF(reverseString.c_str());
}

JNIEXPORT void JNICALL Java_jni_JNIDemo_modifyClassMembers(JNIEnv *env, jobject thisObject) {
	jclass thisClass = env->GetObjectClass(thisObject);
	jfieldID fidNumber = env->GetFieldID(thisClass, "number", "I");
	jint number;
	if (fidNumber != NULL) {
		number = env->GetIntField(thisObject, fidNumber);
		//printf("The number value is %d", number);
		env->SetIntField(thisClass, fidNumber, number * 100);
	}	

	jfieldID fidMessage = env->GetFieldID(thisClass, "message", "Ljava/lang/String;");
	jobject message;
	if (fidMessage != NULL) {
		message = env->GetObjectField(thisObject, fidMessage);
		const char* cStr = env->GetStringUTFChars((jstring)message, NULL);
		if (NULL == cStr) return;
			//printf("In C, the string is %s\n", cStr);
			env->ReleaseStringUTFChars((jstring)message, cStr);
			message = env->NewStringUTF("Hello from C");
			if (NULL == message) return;
			env->SetObjectField(thisObject, fidMessage, message);
	}
}

JNIEXPORT void JNICALL Java_jni_JNIDemo_modifyStaticClassMembers(JNIEnv *env, jobject thisObject) {
	jclass thisClass = env->GetObjectClass(thisObject);
	jfieldID fidNumber = env->GetStaticFieldID(thisClass, "staticNumber", "I");
	jint number;
	if (fidNumber != NULL) {
		number = env->GetStaticIntField(thisClass, fidNumber);
		//printf("The number value is %d", number);
	}
	env->SetStaticIntField(thisClass, fidNumber, number * 100);
}

JNIEXPORT void JNICALL Java_jni_JNIDemo_nativeMethod(JNIEnv *env, jobject thisObject) {
	jclass thisClass = env->GetObjectClass(thisObject);

	jmethodID callbackMethod = env->GetMethodID(thisClass, "callback", "()V");
	if (callbackMethod != NULL) {
		env->CallVoidMethod(thisClass, callbackMethod);
	}

	jmethodID callbackMethodByArgs = env->GetMethodID(thisClass, "callbackWithArgs", "(Ljava/lang/String;)V");
	if (callbackMethodByArgs != NULL) {
		jstring message = env->NewStringUTF("Hello from C");
		env->CallVoidMethod(thisClass, callbackMethodByArgs, message);
	}

	jmethodID callbackMethodByArgsAndReturn = env->GetMethodID(thisClass, "callbackWithArgsReturnType", "(II)I");
	if (callbackMethodByArgsAndReturn != NULL) {
		jint sum = env->CallIntMethod(thisClass, callbackMethodByArgsAndReturn, 10, 10);
		printf("In C, Sum is %d", sum);
	}
}
