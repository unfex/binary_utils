#pragma once

extern "C" {
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

typedef bool Bool;
typedef unsigned char Byte;
typedef char SignedByte;
typedef uint16_t Short;
typedef int16_t SignedShort;
typedef uint32_t Triad;
typedef uint32_t Int;
typedef int32_t SignedInt;
typedef uint64_t Long;
typedef int64_t SignedLong;
typedef float Float;
typedef double Double;

#define readValue(stream, type) (*castPointer(stream->buffer[stream->offset], type))
#define writeValue(stream, value) (memcpy(&stream->buffer[stream->size], castPointer(value, char), sizeof(value)))
#define writeReverseValue(stream, value) (reverseCopy(&stream->buffer[stream->size], castPointer(value, char), sizeof(value)))
#define writeBytes(stream, source, length) (memcpy(&stream->buffer[stream->size], source, length))
#define writeReverseBytes(stream, source, length) (reverseCopy(&stream->buffer[stream->size], source, length))
#define asBinaryStream(zv)  binary_stream_from_zend(Z_OBJ_P((zv)))
#define getThisStream() (asBinaryStream(getThis()))
#define castPointer(value, type) ((type*)(void*)&value)
#define cast(value, type) (*(type*)(char*)(Z_STRVAL_P(value)))
#define reverseValue(value) (reverse((char*)&value, sizeof(value)))
#define asBinary(zv) binary_from_zend(Z_OBJ_P(zv))

struct binary {
    zend_object std;
};

struct binary_stream  {
    char* buffer;
    Long size;
    Long offset;
    Long capacity;
    zend_object std;
};

static zend_class_entry *binary_ce;
static zend_object_handlers binary_handlers;

static zend_class_entry *binary_stream_ce;
static zend_object_handlers binary_stream_handlers;

template<typename type>
char* address(type& value);
void reverse(char* beginSrc, Long size);
void reverseCopy(char* beginDest, char* beginSrc, Long length);
binary *binary_from_zend(zend_object *obj);
zend_object *binary_new(zend_class_entry *ce);
void binary_destroy(zend_object *object);
void binary_free(zend_object *object);

template<typename type>
type readReverseValue(binary_stream* stream);
bool updateCapacity(binary_stream* stream, Long addLength);
binary_stream *binary_stream_from_zend(zend_object *obj);
zend_object *binary_stream_new(zend_class_entry *ce);
void binary_stream_destroy(zend_object *object);
void binary_stream_free(zend_object *object);

PHP_METHOD(binary, writeBool);
PHP_METHOD(binary, readBool);
PHP_METHOD(binary, signByte);
PHP_METHOD(binary, unsignByte);
PHP_METHOD(binary, writeByte);
PHP_METHOD(binary, readByte);
PHP_METHOD(binary, readSignedByte);
PHP_METHOD(binary, writeShort);
PHP_METHOD(binary, readShort);
PHP_METHOD(binary, readSignedShort);
PHP_METHOD(binary, writeLShort);
PHP_METHOD(binary, readLShort);
PHP_METHOD(binary, readSignedLShort);
PHP_METHOD(binary, writeTriad);
PHP_METHOD(binary, readTriad);
PHP_METHOD(binary, writeLTriad);
PHP_METHOD(binary, readLTriad);
PHP_METHOD(binary, writeInt);
PHP_METHOD(binary, readInt);
PHP_METHOD(binary, writeLInt);
PHP_METHOD(binary, readLInt);
PHP_METHOD(binary, readSignedInt);
PHP_METHOD(binary, readSignedLInt);
PHP_METHOD(binary, writeFloat);
PHP_METHOD(binary, readFloat);
PHP_METHOD(binary, readRoundedFloat);
PHP_METHOD(binary, writeLFloat);
PHP_METHOD(binary, readLFloat);
PHP_METHOD(binary, readRoundedLFloat);
PHP_METHOD(binary, printFloat);
PHP_METHOD(binary, writeDouble);
PHP_METHOD(binary, readDouble);
PHP_METHOD(binary, writeLDouble);
PHP_METHOD(binary, readLDouble);
PHP_METHOD(binary, writeLong);
PHP_METHOD(binary, readLong);
PHP_METHOD(binary, writeLLong);
PHP_METHOD(binary, readLLong);
PHP_METHOD(binary, writeVarInt);
PHP_METHOD(binary, readVarInt);
PHP_METHOD(binary, writeUnsignedVarInt);
PHP_METHOD(binary, readUnsignedVarInt);
PHP_METHOD(binary, writeVarLong);
PHP_METHOD(binary, readVarLong);
PHP_METHOD(binary, writeUnsignedVarLong);
PHP_METHOD(binary, readUnsignedVarLong);

PHP_METHOD(binary_stream, __construct);
PHP_METHOD(binary_stream, rewind);
PHP_METHOD(binary_stream, reset);
PHP_METHOD(binary_stream, setOffset);
PHP_METHOD(binary_stream, getOffset);
PHP_METHOD(binary_stream, setBuffer);
PHP_METHOD(binary_stream, getBuffer);
PHP_METHOD(binary_stream, get);
PHP_METHOD(binary_stream, getRemaining);
PHP_METHOD(binary_stream, put);
PHP_METHOD(binary_stream, getBool);
PHP_METHOD(binary_stream, putBool);
PHP_METHOD(binary_stream, getByte);
PHP_METHOD(binary_stream, putByte);
PHP_METHOD(binary_stream, getShort);
PHP_METHOD(binary_stream, getSignedShort);
PHP_METHOD(binary_stream, putShort);
PHP_METHOD(binary_stream, getLShort);
PHP_METHOD(binary_stream, getSignedLShort);
PHP_METHOD(binary_stream, putLShort);
PHP_METHOD(binary_stream, getTriad);
PHP_METHOD(binary_stream, putTriad);
PHP_METHOD(binary_stream, getLTriad);
PHP_METHOD(binary_stream, putLTriad);
PHP_METHOD(binary_stream, getInt);
PHP_METHOD(binary_stream, putInt);
PHP_METHOD(binary_stream, getLInt);
PHP_METHOD(binary_stream, putLInt);
PHP_METHOD(binary_stream, getFloat);
PHP_METHOD(binary_stream, getRoundedFloat);
PHP_METHOD(binary_stream, putFloat);
PHP_METHOD(binary_stream, getLFloat);
PHP_METHOD(binary_stream, getRoundedLFloat);
PHP_METHOD(binary_stream, putLFloat);
PHP_METHOD(binary_stream, getDouble);;
PHP_METHOD(binary_stream, putDouble);
PHP_METHOD(binary_stream, getLDouble);
PHP_METHOD(binary_stream, putLDouble);
PHP_METHOD(binary_stream, getLong);
PHP_METHOD(binary_stream, putLong);
PHP_METHOD(binary_stream, getLLong);
PHP_METHOD(binary_stream, putLLong);
PHP_METHOD(binary_stream, getUnsignedVarInt);
PHP_METHOD(binary_stream, putUnsignedVarInt);
PHP_METHOD(binary_stream, getVarInt);
PHP_METHOD(binary_stream, putVarInt);
PHP_METHOD(binary_stream, getUnsignedVarLong);
PHP_METHOD(binary_stream, putUnsignedVarLong);
PHP_METHOD(binary_stream, getVarLong);
PHP_METHOD(binary_stream, putVarLong);
PHP_METHOD(binary_stream, feof);

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_none, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_one, 0, 0, 1)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_two, 0, 0, 2)
ZEND_ARG_INFO(0, value)
ZEND_ARG_INFO(1, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_stream_none, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_stream_one, 0, 0, 1)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_binary_stream_two, 0, 0, 2)
ZEND_ARG_INFO(0, buffer)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

zend_function_entry binary_methods[] = {
	PHP_ME(binary, writeBool, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readBool, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, signByte, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, unsignByte, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeByte, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readByte, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readSignedByte, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readSignedShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readSignedLShort, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeTriad, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readTriad, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLTriad, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLTriad, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readSignedInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readSignedLInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readRoundedFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readRoundedLFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, printFloat, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeDouble, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readDouble, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLDouble, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLDouble, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeLLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readLLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeVarInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readVarInt, arginfo_binary_two, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeUnsignedVarInt, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readUnsignedVarInt, arginfo_binary_two, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeVarLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readVarLong, arginfo_binary_two, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, writeUnsignedVarLong, arginfo_binary_one, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_ME(binary, readUnsignedVarLong, arginfo_binary_two, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry binary_stream_methods[] = {
    PHP_ME(binary_stream, __construct, arginfo_binary_stream_two, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, rewind, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, reset, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, setOffset, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getOffset, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, setBuffer, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getBuffer, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, get, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getRemaining, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, put, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getBool, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putBool, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getByte, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putByte, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getShort, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getSignedShort, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putShort, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLShort, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getSignedLShort, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLShort, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getTriad, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putTriad, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLTriad, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLTriad, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getInt, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putInt, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLInt, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLInt, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getFloat, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getRoundedFloat, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putFloat, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLFloat, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getRoundedLFloat, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLFloat, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getDouble, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putDouble, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLDouble, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLDouble, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLong, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLong, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getLLong, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putLLong, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getUnsignedVarInt, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putUnsignedVarInt, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getVarInt, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putVarInt, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getUnsignedVarLong, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putUnsignedVarLong, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, getVarLong, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, putVarLong, arginfo_binary_stream_one, ZEND_ACC_PUBLIC)
    PHP_ME(binary_stream, feof, arginfo_binary_stream_none, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(binary_utils);
PHP_MSHUTDOWN_FUNCTION(binary_utils);
