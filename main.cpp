//
//  Copyright (c) 2017 Hintay <hintay@me.com>.
//  All rights reserved.
//

#include "i18n/I18nUtils.h"
#include <Windows.h>
#include <codecvt>

#include "tp_stub.h"

USING_NS_I18N;

std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

//---------------------------------------------------------------------------
// テスト関数 もういっこ
//---------------------------------------------------------------------------
class tLoadMoFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *LoadMoFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tLoadMoFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	// 引数 : ディレクトリ
	if (numparams < 1) return TJS_E_BADPARAMCOUNT;

	IStream *st = TVPCreateIStream(ttstr(param[0]->AsStringNoAddRef()), TJS_BS_READ);
	if (st) {
		// 读取文件大小
		STATSTG stat;
		st->Stat(&stat, STATFLAG_NONAME);
		ULONG FileSize = (ULONG)stat.cbSize.QuadPart;
		unsigned char *data = new unsigned char[FileSize];

		// 读取数据
		if (st->Read(data, FileSize, &FileSize) == S_OK) {
			string domain = I18nUtils::DEFAULT_DOMAIN; 
			if (numparams == 2)
				domain = conv.to_bytes(param[1]->GetString());
			I18nUtils::getInstance()->addMO(data, domain);
			delete[] data;
			st->Release();
			return TJS_S_TRUE;
		}
	}
	return TJS_S_FALSE;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tGettextFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *GettextFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tGettextFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	// 引数 : ディレクトリ
	if (numparams < 1) return TJS_E_BADPARAMCOUNT;

	if (!result || param[0]->Type() == tvtVoid)
		return TJS_S_OK;

	// 输入字符为空时可能会导致崩溃，提前进行处理
	const auto input_string = param[0]->GetString();
	if (input_string == nullptr)
	{
		*result = "";
		return TJS_S_OK;
	}

	const auto text = conv.to_bytes(input_string);

	// 返回 utf-8 数据
	const auto return_text = I18nUtils::getInstance()->gettext(text, numparams == 2 ? conv.to_bytes(param[1]->GetString()) : I18nUtils::DEFAULT_DOMAIN);

	*result = conv.from_bytes(return_text).c_str();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tXGettextFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *XGettextFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tXGettextFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	// 引数 : ディレクトリ
	if (numparams < 2) return TJS_E_BADPARAMCOUNT;

	if (!result)
		return TJS_S_OK;

	if (!result || param[0]->Type() == tvtVoid)
		return TJS_S_OK;

	// 输入字符为空时可能会导致崩溃，提前进行处理
	const auto input_string = param[0]->GetString();
	if (input_string == nullptr)
	{
		*result = "";
		return TJS_S_OK;
	}

	const string text = conv.to_bytes(input_string);
	const string context = conv.to_bytes(param[1]->GetString());

	// 返回 utf-8 数据
	const std::string return_text = I18nUtils::getInstance()->xgettext(text, context, numparams == 3 ? conv.to_bytes(param[2]->GetString()) : I18nUtils::DEFAULT_DOMAIN);

	*result = conv.from_bytes(return_text).c_str();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tNGettextFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *NGettextFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tNGettextFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	// 引数 : ディレクトリ
	if (numparams < 3) return TJS_E_BADPARAMCOUNT;

	if (!result)
		return TJS_S_OK;

	// 第三个参数应该为整数
	if (param[2]->Type() != tvtInteger) return TJS_E_INVALIDPARAM;

	if (!result || param[0]->Type() == tvtVoid)
		return TJS_S_OK;

	// 输入字符为空时可能会导致崩溃，提前进行处理
	string singular = "";
	string plural = "";
	if (param[0]->GetString() != nullptr) singular = conv.to_bytes(param[0]->GetString());
	if (param[1]->GetString() != nullptr) plural = conv.to_bytes(param[1]->GetString());

	// 返回 utf-8 数据
	const std::string return_text = I18nUtils::getInstance()->ngettext(singular, plural, tjs_int(param[2]->AsInteger()), numparams == 4 ? conv.to_bytes(param[3]->GetString()) : I18nUtils::DEFAULT_DOMAIN);

	*result = conv.from_bytes(return_text).c_str();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tNXGettextFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *NXGettextFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tNXGettextFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	// 引数 : ディレクトリ
	if (numparams < 4) return TJS_E_BADPARAMCOUNT;

	if (!result)
		return TJS_S_OK;

	// 第三个参数应该为整数
	if (param[2]->Type() != tvtInteger) return TJS_E_INVALIDPARAM;

	// 第四个参数应该为字符串
	if (param[3]->Type() != tvtString) return TJS_E_INVALIDPARAM;

	// 输入字符为空时可能会导致崩溃，提前进行处理
	string singular = "";
	string plural = "";
	if (param[0]->GetString() != nullptr) singular = conv.to_bytes(param[0]->GetString());
	if (param[1]->GetString() != nullptr) plural = conv.to_bytes(param[1]->GetString());

	// 返回 utf-8 数据
	const std::string return_text = I18nUtils::getInstance()->nxgettext(singular, plural, tjs_int(param[2]->AsInteger()), conv.to_bytes(param[3]->GetString()), numparams == 5 ? conv.to_bytes(param[4]->GetString()) : I18nUtils::DEFAULT_DOMAIN);

	*result = conv.from_bytes(return_text).c_str();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tRemoveMOFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *RemoveMOFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tRemoveMOFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	string domain = I18nUtils::DEFAULT_DOMAIN;
	if (numparams == 1)
		domain = conv.to_bytes(param[0]->GetString());

	I18nUtils::getInstance()->removeMO(domain);

	return TJS_S_OK;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class tRemoveAllMOFunction : public tTJSDispatch
{
	// 例として、与えられた引数の合計を求める関数
	// (引数の中に実数があれば答えも実数、そうでなければ答えは整数にする)

	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);
} *RemoveAllMOFunction;
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD tRemoveAllMOFunction::FuncCall(
	tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
	tTJSVariant *result,
	tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
{
	if (membername) return TJS_E_MEMBERNOTFOUND;

	I18nUtils::getInstance()->removeAllMO();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason,
	void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;
extern "C" __declspec(dllexport) HRESULT _stdcall V2Link(iTVPFunctionExporter *exporter)
{
	// スタブの初期化(必ず記述する)
	TVPInitImportStub(exporter);

	// AverageFunction の作成と登録
	tTJSVariant val;

	// TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();



	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	LoadMoFunction = new tLoadMoFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(LoadMoFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	LoadMoFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("loadMoFile"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	RemoveMOFunction = new tRemoveMOFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(RemoveMOFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	RemoveMOFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("removeMO"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	RemoveAllMOFunction = new tRemoveAllMOFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(RemoveAllMOFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	RemoveAllMOFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("removeAllMO"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	GettextFunction = new tGettextFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(GettextFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	GettextFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("__"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	XGettextFunction = new tXGettextFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(XGettextFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	XGettextFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("_x"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	NGettextFunction = new tNGettextFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(NGettextFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	NGettextFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("_n"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	// 1 まずオブジェクトを作成
	NXGettextFunction = new tNXGettextFunction();

	// 2 AverageFunction を tTJSVariant 型に変換
	val = tTJSVariant(NXGettextFunction);

	// 3 すでに val が AverageFunction を保持しているので、AverageFunction は
	//   Release する
	NXGettextFunction->Release();


	// 4 global の PropSet メソッドを用い、オブジェクトを登録する
	global->PropSet(
		TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
		TJS_W("_nx"), // メンバ名 ( かならず TJS_W( ) で囲む )
		NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
		&val, // 登録する値
		global // コンテキスト ( global でよい )
		);
	//-----------------------------------------------------------------------


	// - global を Release する
	global->Release();

	// もし、登録する関数が複数ある場合は 1 ～ 4 を繰り返す


	// val をクリアする。
	// これは必ず行う。そうしないと val が保持しているオブジェクト
	// が Release されず、次に使う TVPPluginGlobalRefCount が正確にならない。
	val.Clear();


	// この時点での TVPPluginGlobalRefCount の値を
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// として控えておく。TVPPluginGlobalRefCount はこのプラグイン内で
	// 管理されている tTJSDispatch 派生オブジェクトの参照カウンタの総計で、
	// 解放時にはこれと同じか、これよりも少なくなってないとならない。
	// そうなってなければ、どこか別のところで関数などが参照されていて、
	// プラグインは解放できないと言うことになる。

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) HRESULT _stdcall V2Unlink()
{
	// 吉里吉里側から、プラグインを解放しようとするときに呼ばれる関数。

	// もし何らかの条件でプラグインを解放できない場合は
	// この時点で E_FAIL を返すようにする。
	// ここでは、TVPPluginGlobalRefCount が GlobalRefCountAtInit よりも
	// 大きくなっていれば失敗ということにする。
	if (TVPPluginGlobalRefCount > GlobalRefCountAtInit) return E_FAIL;
	// E_FAIL が帰ると、Plugins.unlink メソッドは偽を返す

	// TJS のグローバルオブジェクトに登録した average 関数などを削除する

	// - まず、TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// - global の DeleteMember メソッドを用い、オブジェクトを削除する
	if (global)
	{
		// TJS 自体が既に解放されていたときなどは
		// global は NULL になり得るので global が NULL でない
		// ことをチェックする

		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("loadMoFile"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("removeMO"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("removeAllMO"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("__"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("_x"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("_n"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("_nx"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
		// 登録した関数が複数ある場合は これを繰り返す
	}

	// - global を Release する
	if (global) global->Release();

	// スタブの使用終了(必ず記述する)
	TVPUninitImportStub();

	// 移除所有载入的mo文件
	I18nUtils::getInstance()->removeAllMO();

	return S_OK;
}
