#ifndef GK_ASSERT_HPP
#define GK_ASSERT_HPP

#define GK_ASSERT(cond, error) { if(!(cond)) { printf error; throw -1; } }
#define GK_CHECK(cond, error) { if(!(cond)) { printf error; } }
#define GK_PRINT(format, ...) printf(format, __VA_ARGS__);

#endif // GK_ASSERT_HPP