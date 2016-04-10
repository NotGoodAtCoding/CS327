#ifndef DICE_H
#define DICE_H

# ifdef __cplusplus
extern "C" {
# endif

//int roll(dice d);

# ifdef __cplusplus
}
class dice {
public:
    int base;
    int num;
    int sides;

    dice(void);
    dice(int base, int num, int sides);

};

#endif

#endif //CS327_DICE_H
