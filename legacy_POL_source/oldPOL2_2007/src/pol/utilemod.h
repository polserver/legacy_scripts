#ifndef UTILEMOD_H
#define UTILEMOD_H

class UtilExecutorModule : public TmplExecutorModule<UtilExecutorModule>
{
public:
    UtilExecutorModule( Executor& exec ) : TmplExecutorModule<UtilExecutorModule>("util",exec) {}

    BObjectImp* mf_RandomInt();
    BObjectImp* mf_RandomFloat();
    BObjectImp* mf_RandomDiceRoll();
	BObjectImp* mf_StrFormatTime();
};

#endif
