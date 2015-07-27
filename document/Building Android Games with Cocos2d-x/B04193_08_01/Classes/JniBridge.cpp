#include "JniBridge.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#define CLASS_NAME "com/packtpub/jni/JniFacade"
#define METHOD_NAME "showToast"
#define PARAM_CODE "(Ljava/lang/String;)V"

USING_NS_CC;

void JniBridge::showToast(const char* message)
{
	JniMethodInfo method;
	JniHelper::getStaticMethodInfo(method, CLASS_NAME,METHOD_NAME,PARAM_CODE);
	jstring stringMessage = method.env->NewStringUTF(message);
    method.env->CallStaticVoidMethod(method.classID, method.methodID, stringMessage);
    method.env->DeleteLocalRef(stringMessage);
}
#endif

