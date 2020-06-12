class FileExecutorModule;

typedef BObjectImp* (FileExecutorModule::*FileExecutorModuleFn)();

struct FileFunctionDef
{
	char funcname[ 33 ];
	FileExecutorModuleFn fptr;
};
 

class FileExecutorModule : public ExecutorModule
{
  public:
	BObjectImp* FileOpen();
	BObjectImp* FileClose();
	BObjectImp* FileRead();
	BObjectImp* FileWrite();
	BObjectImp* FileSeek();
	BObjectImp* FileTell();
	// Object *FileChSize();

    explicit FileExecutorModule(Executor& exec) : ExecutorModule("File", exec) { }

	// class machinery
  protected:
	virtual BObjectImp* execFunc( unsigned idx );
	virtual int functionIndex( const char *func );
	static FileFunctionDef function_table[];
};
