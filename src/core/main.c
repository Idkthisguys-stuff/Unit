#include "limine.h"
#include <stddef.h>
#include <stdint.h>

extern void gdt_init(void);
extern void idt_init(void);

struct limine_framebuffer *global_fb;
uint32_t *global_fb_ptr;

__attribute__((used,
               section(".limine_requests_start"))) static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};



void _start(void) {
    gdt_init();
    idt_init();

    __asm__ volatile("sti");

    global_fb = framebuffer_request.response->framebuffers[0];
    global_fb_ptr = (uint32_t *)global_fb->address;

  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    for (;;) {
      __asm__("hlt");
    }
  }

  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
  uint32_t *fb_ptr = (uint32_t *)fb->address;

  for (size_t y = 0; y < 100; y++) {
    for (size_t x = 0; x < 100; x++) {
      fb_ptr[y * (fb->pitch / 4) + x] = 0xFFFFFFFF;
    }
  }

  for (;;) {
    __asm__("hlt");
  }
}