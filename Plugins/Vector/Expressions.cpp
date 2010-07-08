// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Built-in expressions
//////////////////////////////////////////////////////////////////////////////////

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyObject(3,7).  You can check ExpReturn::Type() or GetNumParams() to check for overloads.
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.  OF_UNDEFINEDEXPRESSIONS must be specified in Main.h.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////
bool invalidVector(const ExpReturn& vec)
{
	if( vec.Type() != EXPTYPE_ARRAY)
		return true;

	ExpStore* arr = vec.GetArray();
	int arr_size = vec.GetArraySize();

	if(arr_size != 2)
		return true;

	// Okay good to go!
	return false;
}

float getVectorX(const ExpStore* vec)
{
	return vec->GetFloat();
}

float getVectorY(const ExpStore* vec)
{
	vec++;
	return vec->GetFloat();
}

cr::point getVector(const ExpStore* vec)
{
	return cr::point(vec->GetFloat(), (vec+1)->GetFloat());
}


long ExtObject::eSetX(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	ExpStore* vector1 = params[0].GetArray();
	
	static ExpStore vectorReturn[2] = {params[1].GetFloat(), getVectorY(vector1) };	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eSetY(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	ExpStore* vector1 = params[0].GetArray();

	static ExpStore vectorReturn[2] = {getVectorX(vector1), params[1].GetFloat() };	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eSetLength(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v = getVector(params[0].GetArray());
	v.distance( params[1].GetFloat() );

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eSetAngle(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v = getVector(params[0].GetArray());
	v.angle( cr::to_radians(params[1].GetFloat()) );

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}


long ExtObject::eGetX(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	ExpStore* vector1 = params[0].GetArray();

	return ret = getVectorX(vector1);	
}

long ExtObject::eGetY(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	ExpStore* vector1 = params[0].GetArray();

	return ret = getVectorY(vector1);	
}

long ExtObject::eGetLength(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v = getVector(params[0].GetArray());

	return ret = v.distance();
}

long ExtObject::eGetAngle(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v = getVector(params[0].GetArray());

	return ret = cr::to_degrees( v.angle() );
}


long ExtObject::eAdd(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) 
	 || invalidVector(params[1]) )
		return 0;

	cr::point v = getVector(params[0].GetArray())  +   getVector(params[1].GetArray());

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eSubtract(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) 
	 || invalidVector(params[1]) )
		return 0;

	cr::point v = getVector(params[0].GetArray())  -   getVector(params[1].GetArray());

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eMultiply(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v;

	if( invalidVector(params[1]) )
		v = getVector(params[0].GetArray())  *   getVector(params[1].GetArray());
	else
		v = getVector(params[0].GetArray()) * params[1].GetFloat();

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eDivide(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) )
		return 0;

	cr::point v;

	if( invalidVector(params[1]) )
		v = getVector(params[0].GetArray())  /   getVector(params[1].GetArray());
	else
		v = getVector(params[0].GetArray()) / params[1].GetFloat();

	static ExpStore vectorReturn[2] = {v.x, v.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eDot(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) 
	 || invalidVector(params[1]) )
		return 0;

	cr::point v1 = getVector(params[0].GetArray());
	cr::point v2 = getVector(params[1].GetArray());

	return ret = v1.x * v2.x + v1.y * v2.y;
}


long ExtObject::eLerp(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) 
		|| invalidVector(params[1]) )
		return 0;

	cr::point v1 = getVector(params[0].GetArray());
	cr::point v2 = getVector(params[1].GetArray());
	float r = params[2].GetFloat();

	static ExpStore vectorReturn[2] = { v1.x * (1-r) + v2.x * r, v1.y * (1-r) + v2.y};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eDistance(LPVAL params, ExpReturn& ret)
{
	if( invalidVector(params[0]) 
		|| invalidVector(params[1]) )
		return 0;

	cr::point v = getVector(params[0].GetArray()) - getVector(params[1].GetArray());

	return ret = v.distance();
}


long ExtObject::eLeft(LPVAL params, ExpReturn& ret)
{
	static ExpStore vectorReturn[2] = { -1.0f, 0.0f};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eRight(LPVAL params, ExpReturn& ret)
{
	static ExpStore vectorReturn[2] = { 1.0f, 0.0f};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eUp(LPVAL params, ExpReturn& ret)
{
	static ExpStore vectorReturn[2] = { 0.0f, -1.0f};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eDown(LPVAL params, ExpReturn& ret)
{
	static ExpStore vectorReturn[2] = { 0.0f, 1.0f};	
	return ret.ReturnArray(vectorReturn, 2);
}

long ExtObject::eOne(LPVAL params, ExpReturn& ret)
{
	return ret = 0;
}

long ExtObject::eZero(LPVAL params, ExpReturn& ret)
{
	return ret = 0;
}

