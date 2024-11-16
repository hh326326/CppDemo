#ifndef __NonCopyable_H__
#define __NonCopyable_H__


class NonCopyable
{
/* public: */
protected:
    NonCopyable() = default;
    NonCopyable(const NonCopyable &rhs) = delete;
    NonCopyable operator=(const NonCopyable &rhs) = delete;

private:

};


#endif
