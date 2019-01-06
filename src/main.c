/* AnalogStickDisable Ver1.1

 Hack-Usagi(https://github.com/Hack-Usagi)
  idea
  developed

 Princess-of-Sleeping (https://github.com/Princess-of-Sleeping)
  Advice on development
  Beta Test

*/

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/ctrl.h>
#include <taihen.h>

#define HookExport(module_name, library_nid, func_nid, func_name) ({ \
	taiHookFunctionExportForKernel(KERNEL_PID, &func_name ## _ref, module_name, library_nid, func_nid, func_name ## _patch); \
})

#define HookRelease(hook_uid, func_name) ({ \
	if(hook_uid >= 0)taiHookReleaseForKernel(hook_uid, func_name ## _ref); \
})

#define CTRL_PATCH_FUNC(func_name)	\
	static tai_hook_ref_t func_name ## _ref; \
	static int func_name ## _patch(int port, SceCtrlData *ctrl, int count){ \
	int state; \
	SceCtrlData kctrl; \
	ENTER_SYSCALL(state); \
	int ret = TAI_CONTINUE(int, func_name ## _ref, port, ctrl, count); \
	ksceKernelMemcpyUserToKernel(&kctrl, (uintptr_t)ctrl, sizeof(SceCtrlData) ); \
	kctrl.lx = 0x80; \
	kctrl.ly = 0x80; \
	kctrl.rx = 0x80; \
	kctrl.ry = 0x80; \
        ksceKernelMemcpyKernelToUser((uintptr_t)ctrl, &kctrl, sizeof(SceCtrlData) ); \
	EXIT_SYSCALL(state); \
	return ret; \
	}

CTRL_PATCH_FUNC(sceCtrlPeekBufferPositive)
CTRL_PATCH_FUNC(sceCtrlReadBufferPositive)
CTRL_PATCH_FUNC(sceCtrlPeekBufferPositive2)
CTRL_PATCH_FUNC(sceCtrlReadBufferPositive2)

CTRL_PATCH_FUNC(sceCtrlPeekBufferPositiveExt)
CTRL_PATCH_FUNC(sceCtrlReadBufferPositiveExt)
CTRL_PATCH_FUNC(sceCtrlPeekBufferPositiveExt2)
CTRL_PATCH_FUNC(sceCtrlReadBufferPositiveExt2)

CTRL_PATCH_FUNC(sceCtrlPeekBufferNegative)
CTRL_PATCH_FUNC(sceCtrlReadBufferNegative)
CTRL_PATCH_FUNC(sceCtrlPeekBufferNegative2)
CTRL_PATCH_FUNC(sceCtrlReadBufferNegative2)

static SceUID uid[0xC];

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){

	uid[0x0] = HookExport("SceCtrl", 0xD197E3C7, 0xA9C3CED6, sceCtrlPeekBufferPositive);
	uid[0x1] = HookExport("SceCtrl", 0xD197E3C7, 0x15F81E8C, sceCtrlPeekBufferPositive2);

	uid[0x2] = HookExport("SceCtrl", 0xD197E3C7, 0xA59454D3, sceCtrlPeekBufferPositiveExt);
	uid[0x3] = HookExport("SceCtrl", 0xD197E3C7, 0x860BF292, sceCtrlPeekBufferPositiveExt2);

	uid[0x4] = HookExport("SceCtrl", 0xD197E3C7, 0x67E7AB83, sceCtrlReadBufferPositive);
	uid[0x5] = HookExport("SceCtrl", 0xD197E3C7, 0xC4226A3E, sceCtrlReadBufferPositive2);

	uid[0x6] = HookExport("SceCtrl", 0xD197E3C7, 0xE2D99296, sceCtrlReadBufferPositiveExt);
	uid[0x7] = HookExport("SceCtrl", 0xD197E3C7, 0xA7178860, sceCtrlReadBufferPositiveExt2);

	uid[0x8] = HookExport("SceCtrl", 0xD197E3C7, 0x104ED1A7, sceCtrlPeekBufferNegative);
	uid[0x9] = HookExport("SceCtrl", 0xD197E3C7, 0x15F96FB0, sceCtrlReadBufferNegative);

	uid[0xA] = HookExport("SceCtrl", 0xD197E3C7, 0x81A89660, sceCtrlPeekBufferNegative2);
	uid[0xB] = HookExport("SceCtrl", 0xD197E3C7, 0x27A0C5FB, sceCtrlReadBufferNegative2);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args){

	HookRelease(uid[0x0], sceCtrlPeekBufferPositive);
	HookRelease(uid[0x1], sceCtrlReadBufferPositive);
	HookRelease(uid[0x2], sceCtrlPeekBufferPositive2);
	HookRelease(uid[0x3], sceCtrlReadBufferPositive2);
	HookRelease(uid[0x4], sceCtrlPeekBufferPositiveExt);
	HookRelease(uid[0x5], sceCtrlReadBufferPositiveExt);
	HookRelease(uid[0x6], sceCtrlPeekBufferPositiveExt2);
	HookRelease(uid[0x7], sceCtrlReadBufferPositiveExt2);
	HookRelease(uid[0x8], sceCtrlPeekBufferNegative);
	HookRelease(uid[0x9], sceCtrlReadBufferNegative);
	HookRelease(uid[0xA], sceCtrlPeekBufferNegative2);
	HookRelease(uid[0xB], sceCtrlReadBufferNegative2);

	return SCE_KERNEL_STOP_SUCCESS;
}
