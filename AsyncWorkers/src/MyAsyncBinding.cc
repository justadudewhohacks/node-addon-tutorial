#include "MyAsyncBinding.h"

NAN_MODULE_INIT(MyAsyncBinding::Init) {
  Nan::SetMethod(target, "doSyncStuff", DoSyncStuff);
  Nan::SetMethod(target, "doAsyncStuff", DoAsyncStuff);
}

void delay(int iterations) {
  // use volatile to prevent compiler from optimizing empty loop
  for (volatile int i = 0; i < iterations; i++);
}

NAN_METHOD(MyAsyncBinding::DoSyncStuff) {
  if(!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("expected arg 0: string workerId").ToLocalChecked());
  }
  if(!info[1]->IsInt32()) {
    return Nan::ThrowError(Nan::New("expected arg 1: int iterations").ToLocalChecked());
  }

  std::string workerId = std::string(*Nan::Utf8String(info[0]->ToString()));
  int iterations = info[1]->Int32Value();

  // mimic long running task
  delay(iterations);

  info.GetReturnValue().Set(Nan::New(workerId).ToLocalChecked());
}

class MyAsyncWorker : public Nan::AsyncWorker {
public:
  std::string workerId;
	int iterations;
  bool throwsError;

	MyAsyncWorker(std::string workerId, int iterations, bool throwsError, Nan::Callback *callback)
    : Nan::AsyncWorker(callback) {

    this->workerId = workerId;
    this->iterations = iterations;
    this->throwsError = throwsError;
  }

	void Execute() {
		if (throwsError) {
			this->SetErrorMessage("An error occured!");
      return;
		}

    // mimic long running task
    delay(iterations);
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
      Nan::Null(), // no error occured
      Nan::New(workerId).ToLocalChecked()
    };
    Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 2, argv);
	}

	void HandleErrorCallback() {
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
      Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
      Nan::Null()
    };
    Nan::Call(callback->GetFunction(), Nan::GetCurrentContext()->Global(), 2, argv);
  }
};

NAN_METHOD(MyAsyncBinding::DoAsyncStuff) {
  if(!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("expected arg 0: string workerId").ToLocalChecked());
  }
  if(!info[1]->IsInt32()) {
    return Nan::ThrowError(Nan::New("expected arg 1: int iterations").ToLocalChecked());
  }
  if(!info[2]->IsBoolean()) {
    return Nan::ThrowError(Nan::New("expected arg 2: bool throwsError").ToLocalChecked());
  }
  if(!info[3]->IsFunction()) {
    return Nan::ThrowError(Nan::New("expected arg 3: function callback").ToLocalChecked());
  }

  // starting the async worker
	Nan::AsyncQueueWorker(new MyAsyncWorker(
    std::string(*Nan::Utf8String(info[0]->ToString())),
    info[1]->Int32Value(),
    info[2]->BooleanValue(),
		new Nan::Callback(info[3].As<v8::Function>())
	));
}
