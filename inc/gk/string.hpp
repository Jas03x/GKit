#ifndef GK_STRING_HPP
#define GK_STRING_HPP

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t') || ((x) == '\n'))

#define IS_ALPHA(x) ((((x) >= 'a') && ((x) <= 'z')) || (((x) >= 'A') && ((x) <= 'Z')))
#define IS_NUM(x) (((x) >= '0') && ((x) <= '9'))
#define IS_ALPHA_NUM(x) (IS_ALPHA(x) || IS_NUM(x) || ((x) == '_'))

#endif // GK_STRING_HPP
