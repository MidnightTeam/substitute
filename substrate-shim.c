#include <stdlib.h>
#include <objc/runtime.h>

extern void *SubGetImageByName(const char *filename) __asm__("SubGetImageByName");;
void *MSGetImageByName(const char *filename) {
	    return SubGetImageByName(filename);
}

extern void *SubFindSymbol(void *image, const char *name) __asm__("SubFindSymbol");
void *MSFindSymbol(void *image, const char *name) {
		return SubFindSymbol(image, name);
}

extern void SubHookFunction(void *symbol, void *replace, void **result) __asm__("SubHookFunction");
void MSHookFunction(void *symbol, void *replace, void **result) {
		SubHookFunction(symbol, replace, result);
}

extern void SubHookMessageEx(Class _class, SEL sel, IMP imp, IMP *result) __asm__("SubHookMessageEx");
void MSHookMessageEx(Class _class, SEL sel, IMP imp, IMP *result) {
		SubHookMessageEx(_class, sel, imp, result);
}

// i don't think anyone uses this function anymore, but it's here for completeness
void MSHookClassPair(Class _class, Class hook, Class old) {
    unsigned int n_methods = 0;
    Method *hooks = class_copyMethodList(hook, &n_methods);

    for (unsigned int i = 0; i < n_methods; ++i) {
        SEL selector = method_getName(hooks[i]);
        const char *what = method_getTypeEncoding(hooks[i]);

        Method old_mptr = class_getInstanceMethod(old, selector);
        Method cls_mptr = class_getInstanceMethod(_class, selector);

        if (cls_mptr) {
            class_addMethod(old, selector, method_getImplementation(hooks[i]), what);
            method_exchangeImplementations(cls_mptr, old_mptr);
        } else {
            class_addMethod(_class, selector, method_getImplementation(hooks[i]), what);
        }
    }

    free(hooks);
}
