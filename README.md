# Darwin OS case study: evidence kit

Collects real Darwin (XNU) output for the report using a free GitHub-hosted macOS runner.

## How to use

1. Create a **public** GitHub repository and push this folder to it.
2. Open the **Actions** tab, select **Darwin evidence**, and click **Run workflow**
   (it also runs on every push).
3. Open the finished run. Each step (Architecture, Memory, IPC, ...) has its own
   collapsible log. Screenshot the step that matches each report section.

## Section-to-evidence map

| Report section | Workflow step / program |
|---|---|
| Architecture / Kernel | Step 1 (`uname`, `sysctl kern.version`), `mach_task_info.c` |
| Process and thread management | Step 2, `fork_demo.c`, `threads_demo.c` |
| CPU scheduling | Step 3, `threads_demo.c` |
| Memory management | Step 4, `vm_demo.c`, `mach_task_info.c` |
| IPC | Step 5, `mach_ipc_demo.c` |
| File system | Step 6 (APFS) |
| Security | Step 7 (SIP, Gatekeeper, code signing) |
| Networking | Step 8 |
| Drivers / I/O Kit | Step 9 |

## Known limits (mention in the report)

- The runner is a headless virtual machine: no GUI, so no Activity Monitor.
- Some commands (`csrutil`, `diskutil apfs list`, `pfctl`) may print limited or
  different output on a virtualized runner. Note this when you show them.
- Run the programs locally on any Mac or in the workflow; they do not build on Windows.

## Also capture (no Mac needed)

- Clone https://github.com/apple-oss-distributions/xnu and screenshot the folders
  `osfmk/` (Mach), `bsd/` (BSD layer), `iokit/` (I/O Kit), plus files such as
  `osfmk/kern/sched_prim.c` and `osfmk/ipc/`.
