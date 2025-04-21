#ifndef CONTESTANT_H
#define CONTESTANT_H



class contestant {
protected:
    bool stunned = 0;
    int hp;
    int max_hp;
    int mp;
    int max_mp;
public:
    void stun();
    void unstun();
    bool move();
    void set_hp(int Hp);
    int get_hp();
    void set_mp(int Mp, int max_Mp);
    int get_mp();
};



#endif //CONTESTANT_H
