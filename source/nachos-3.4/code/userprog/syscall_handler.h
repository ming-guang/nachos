#ifndef H_SYSCALL_HANDLER
#define H_SYSCALL_HANDLER
namespace SyscallFS {
    int Handle(int type);
    int Create();
    int Open();
    int Read();
    int Write();
    int Delete();
    int Seek();
}
#endif
