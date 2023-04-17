#ifndef H_SYSCALL_HANDLER
#define H_SYSCALL_HANDLER
namespace SyscallFS {
    int Handle(int type);
    int Create();
    int Open();
    int Close();
    int Read();
    int Write();
    int Delete();
    int Seek();
}

namespace SyscallProcess {
    int Handle(int type);
    int Exec();
    int Running();
}
#endif
