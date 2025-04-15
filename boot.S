.section ".text.boot" // Make sure the linker puts this at the start of the kernel image

.global _start // Execution starts here

_start:
  // Check processor ID is Zero (executing on main core), else hang
  mrs   x1, mpidr_el1   // Read the CPU ID register (Multiprocessor Affinity register)
  and   x1, x1, #3      // Mask off everything but the lower bits (core ID)
  cbz   x1, 2f          // If x1 == 0 (i.e, core 0), jump to label 2
  // We're not on the main core, so hang in an infinite wait loop

1: 
  wfe                   // Wait for event - low-power idle
  b   1b                // Loop forever (do nothing)
2: // We're on the main core!

  // Set stack to start below our code
  ldr   x1, =_start     // Load address of _start label (top of loaeded binary)
  mov   sp, x1          // Set stack pointer to this address

  // Clean the BSS section
  ldr   x1, =__bss_start  // x1 = Start of BSS section
  ldr   w2, =__bss_size   // w2 = Size of the section
3:
  cbz   w2, 4f            // If size is zero, skip loop
  str   xzr, [x1], #8     // Store zero (xzr) at [x1], then x1 += 8
  sub   w2, w2, #1        // Decrement word count
  cbnz  w3, 3b            // Loop if non-zero

  // Jump to our main () routine in C (make sure it doesn't return)
4:
  bl  main
  // In case it does return, halt the master core too
  b   1b
