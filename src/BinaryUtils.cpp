#include "BinaryUtils.h"

#include <string>
#include <iostream>

template<typename type>
inline char* address(type& value) {
    return (char*)&value;
}

inline void reverse(char* beginSrc, Long size) {
    char* endSrc = beginSrc + size - 1;
    while (endSrc > beginSrc) {
        char tmp = *beginSrc;
        *beginSrc++ = *endSrc;
        *endSrc-- = tmp;
    }
}

inline void reverseCopy(char* beginDest, char* beginSrc, Long length) {
    char* endSrc = beginSrc + length - 1;
    while (endSrc >= beginSrc) {
        *beginDest++ = *endSrc--;
    }
}

inline bool updateCapacity(binary_stream* stream, Long addLength) {
    if (stream->size + addLength >= stream->capacity) {
        stream->capacity = (stream->size + addLength) * 2;
        char* buffer = (char*)emalloc(stream->capacity);
        if (!buffer) {
            zend_throw_exception(zend_exception_get_default(), "Could not allocate more memory for the thread.", 1);
            return false;
        }
        memcpy(buffer, stream->buffer, stream->size);
        stream->buffer = buffer;
    }
    return true;
}

template<typename type>
inline type readReverseValue(binary_stream* stream) {
    Long value = 0;
    for (int i = 0; i < sizeof(type); i++) {
        value = (value | stream->buffer[stream->offset + sizeof(type) - i - 1]) << 8;
    }
    type t = *castPointer(value, type);
    return t;
}

inline binary_stream *binary_stream_from_zend(zend_object *obj) {
    binary_stream* stream = (binary_stream*)((char*)(obj) - XtOffsetOf(binary_stream, std));
    return stream;
}

inline binary *binary_from_zend(zend_object *obj) {
    return (binary*)((char*)(obj) - XtOffsetOf(binary, std));
}

zend_object *binary_new(zend_class_entry *ce) {
    binary *intern = (binary*)ecalloc(1, sizeof(binary) +
        zend_object_properties_size(ce));
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &binary_handlers;
    return &intern->std;
}

void binary_destroy(zend_object *object) {
    zend_objects_destroy_object(object);
}

void binary_free(zend_object *object) {
    zend_object_std_dtor(object);
}

zend_object *binary_stream_new(zend_class_entry *ce) {
    binary_stream *intern = (binary_stream*)emalloc(sizeof(binary_stream) + zend_object_properties_size(ce));
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &binary_stream_handlers;
    intern->capacity = 0;
    intern->size = 0;
    intern->offset = 0;
    intern->buffer = NULL;
    return &intern->std;
}

void binary_stream_destroy(zend_object *object) {
    binary_stream *stream = binary_stream_from_zend(object);
    stream->size = 0;
    stream->capacity = 0;
    stream->offset = 0;
    zend_objects_destroy_object(object);
}

void binary_stream_free(zend_object *object) {
    binary_stream* stream = (binary_stream*)((char*)object - XtOffsetOf(binary_stream, std));
    efree(stream->buffer);
    zend_object_std_dtor(object);
}

PHP_METHOD(binary, writeBool) {
    bool value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "b", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(value), sizeof(Bool));
    }
}

PHP_METHOD(binary, readBool) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Bool)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 1.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_BOOL(return_value, cast(value, Bool));
    }
}

PHP_METHOD(binary, signByte) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_LONG(return_value, (SignedByte)value);
    }
}

PHP_METHOD(binary, unsignByte) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_LONG(return_value, value & 0xFF);
    }
}

PHP_METHOD(binary, writeByte) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(value), sizeof(Byte));
    }
}

PHP_METHOD(binary, readByte) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Byte)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 1.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, Byte));
    }
}

PHP_METHOD(binary, readSignedByte) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Byte)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 1.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, SignedByte));
    }
}

PHP_METHOD(binary, writeShort) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        Short svalue = (Short)value;
        reverseValue(svalue);
        ZVAL_STRINGL(return_value, address(svalue), sizeof(Short));
    }
}

PHP_METHOD(binary, readShort) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 2.", 1);
        ZVAL_NULL(return_value);
    } else {
        Short svalue = cast(value, Short);
        reverseValue(svalue);
        ZVAL_LONG(return_value, svalue);
    }
}

PHP_METHOD(binary, readSignedShort) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 2.", 1);
        ZVAL_NULL(return_value);
    } else {
        SignedShort svalue = cast(value, SignedShort);
        reverseValue(svalue);
        ZVAL_LONG(return_value, svalue);
    }
}

PHP_METHOD(binary, writeLShort) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(value), sizeof(Short));
    }
}

PHP_METHOD(binary, readLShort) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 2.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, Short));
    }
}

PHP_METHOD(binary, readSignedLShort) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 2.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, SignedShort));
    }
}

PHP_METHOD(binary, writeTriad) {
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        longValue = (longValue & 0xFFFFFF) << 40;
        reverseValue(longValue);
        ZVAL_STRINGL(return_value, address(longValue), 3);
    }
}

PHP_METHOD(binary, readTriad) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < 3) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 3.", 1);
        ZVAL_NULL(return_value);
    } else {
        Triad t = 0;
        reverseCopy(address(t), Z_STRVAL_P(value), 3);
        ZVAL_LONG(return_value, t);
    }
}

PHP_METHOD(binary, writeLTriad) {
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(longValue), 3);
    }
}

PHP_METHOD(binary, readLTriad) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < 3) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 1.", 3);
        ZVAL_NULL(return_value);
    } else {
        Triad tvalue = 0;
        memcpy(address(tvalue), Z_STRVAL_P(value), 3);
        ZVAL_LONG(return_value, tvalue);
    }
}

PHP_METHOD(binary, writeInt) {
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        Int value = (Int)longValue;
        reverseValue(value);
        ZVAL_STRINGL(return_value, address(value), sizeof(Int));
    }
}

PHP_METHOD(binary, readInt) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        Int ivalue = cast(value, Int);
        reverseValue(ivalue);
        ZVAL_LONG(return_value, ivalue);
    }
}

PHP_METHOD(binary, writeLInt) {
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(longValue), sizeof(Int));
    }
}

PHP_METHOD(binary, readLInt) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, SignedInt));
    }
}

PHP_METHOD(binary, readSignedInt) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, *static_cast<SignedInt*>(static_cast<void*>(Z_STRVAL_P(value))));
    }
}

PHP_METHOD(binary, readSignedLInt) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        SignedInt i = cast(value, SignedInt);
        reverse(address(i), sizeof(i));
        ZVAL_LONG(return_value, i);
    }
}

PHP_METHOD(binary, writeFloat) {
    Double longFloat;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &longFloat) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        Float fvalue = (Float)(longFloat);
        reverseValue(fvalue);
        ZVAL_STRINGL(return_value, address(fvalue), sizeof(Float));
    }
}

PHP_METHOD(binary, readFloat) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float fvalue = cast(value, Float);
        reverseValue(fvalue);
        ZVAL_DOUBLE(return_value, fvalue);
    }
}

PHP_METHOD(binary, readRoundedFloat) {
    zval *value;
    Long accuracy;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zl", &value, &accuracy) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float fvalue = cast(value, Float);
        reverseValue(fvalue);
        Float factor = pow(10.0, accuracy);
        ZVAL_DOUBLE(return_value, Long(fvalue * factor) / factor);
    }
}

PHP_METHOD(binary, writeLFloat) {
    Double longFloat;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &longFloat) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        Float fvalue = (Float)longFloat;
        ZVAL_STRINGL(return_value, address(fvalue), sizeof(Float));
    }
}

PHP_METHOD(binary, readLFloat) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_DOUBLE(return_value, cast(value, Float));
    }
}

PHP_METHOD(binary, readRoundedLFloat) {
    zval *value;
    Long accuracy;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zl", &value, &accuracy) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 4.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float fvalue = cast(value, Float);
        Float factor = pow(10.0, accuracy);
        ZVAL_DOUBLE(return_value, Long(fvalue * factor) / factor);
    }
}

PHP_METHOD(binary, printFloat) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRING(return_value, std::to_string(*static_cast<Float*>(static_cast<void*>(Z_STRVAL_P(value)))).c_str());
    }
}

PHP_METHOD(binary, writeDouble) {
    Double longFloat;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &longFloat) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        reverseValue(longFloat);
        ZVAL_STRINGL(return_value, address(longFloat), sizeof(Double));
    }
}

PHP_METHOD(binary, readDouble) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Double)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 8.", 1);
        ZVAL_NULL(return_value);
    } else {
        Double dvalue = cast(value, Double);
        reverseValue(dvalue);
        ZVAL_DOUBLE(return_value, dvalue);
    }
}

PHP_METHOD(binary, writeLDouble) {
    Double longFloat;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &longFloat) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(longFloat), sizeof(Double));
    }
}

PHP_METHOD(binary, readLDouble) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Double)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 8.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_DOUBLE(return_value, cast(value, Double));
    }
}

PHP_METHOD(binary, writeLong) {
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        reverse(address(longValue), sizeof(longValue));
        ZVAL_STRINGL(return_value, address(longValue), sizeof(longValue));
    }
}   

PHP_METHOD(binary, readLong) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Long)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 8.", 1);
        ZVAL_NULL(return_value);
    } else {
        Long lvalue = *static_cast<Long*>(static_cast<void*>(Z_STRVAL_P(value)));
        reverse(address(lvalue), sizeof(lvalue));
        ZVAL_LONG(return_value, lvalue);
    }
}

PHP_METHOD(binary, writeLLong) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    } else {
        ZVAL_STRINGL(return_value, address(value), sizeof(Long));
    }
}

PHP_METHOD(binary, readLLong) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    if (Z_STRLEN_P(value) < sizeof(Long)) {
        zend_throw_exception(zend_exception_get_default(), "The length of the string must be greater than or equal to 8.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, cast(value, Long));
    }
}

PHP_METHOD(binary, writeVarInt) {
    char buffer[5];
    int offset = 0;
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    SignedInt svalue = (SignedInt)longValue;
    Int value = (Int)svalue;
    value = (value >> 31) ^ (value << 1);
    value = (value & 1) ? (~value | 1) : value;
	Int remaining = value;
	for (int i = 0; i < 5; ++i){
		if ((remaining >> 7) != 0) {
			buffer[offset++] = (remaining | 0x80) & 0xFF;
		} else {
			buffer[offset++] = (remaining & 0x7f) & 0xFF;
			break;
		}
		remaining = remaining >> 7;
	}
    ZVAL_STRINGL(return_value, address(buffer[0]), offset);
}

PHP_METHOD(binary, readVarInt) {
    zval *value;
    zval* goffset;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &value, &goffset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    ZVAL_DEREF(goffset);
    char* buffer = Z_STRVAL_P(value);
    int offset = Z_LVAL_P(goffset);
    if (offset >= Z_STRLEN_P(value)) {
        zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
        ZVAL_NULL(return_value);
        return;
    }
	Int raw = 0;
	for (int i = 0; i <= 28; i += 7) {
		if (buffer[offset] == 0) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
            ZVAL_LONG(goffset, offset);
            ZVAL_NULL(return_value);
            return;
		}
		Int b = buffer[offset++];
		raw |= ((b & 0x7f) << i);

		if((b & 0x80) == 0){
            break;
		}
	}
    ZVAL_LONG(goffset, offset);
    ZVAL_LONG(return_value, (SignedInt)((raw & 1) ? ((raw << 31) ^ (~raw >> 1)) : ((raw << 31) ^ (raw >> 1))));
}

PHP_METHOD(binary, writeUnsignedVarInt) {
    char buffer[5];
    int offset = 0;
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	Int remaining = (Int)longValue;
	for (int i = 0; i < 5; ++i) {
		if ((remaining >> 7) != 0) {
			buffer[offset++] = (remaining | 0x80) & 0xFF;
		} else {
			buffer[offset++] = (remaining & 0x7f) & 0xFF;
			break;
		}
		remaining = remaining >> 7;
	}
    ZVAL_STRINGL(return_value, address(buffer[0]), offset);
}

PHP_METHOD(binary, readUnsignedVarInt) {
    zval *b;
    zval* goffset;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &b, &goffset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    ZVAL_DEREF(goffset);

    char* buffer = Z_STRVAL_P(b);
    int offset = Z_LVAL_P(goffset);
    if (offset >= Z_STRLEN_P(b)) {
        zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
        ZVAL_NULL(return_value);
        return;
    }
	Int value = 0;
	for (int i = 0; i <= 28; i += 7) {
		if (buffer[offset] == 0) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
            ZVAL_LONG(goffset, offset);
            ZVAL_NULL(return_value);
		}
		Int b = buffer[offset++];
		value |= ((b & 0x7f) << i);

		if((b & 0x80) == 0){
            break;
		}
	}
    ZVAL_LONG(goffset, offset);
    ZVAL_LONG(return_value, value);
}

PHP_METHOD(binary, writeVarLong) {
    char buffer[10];
    int offset = 0;
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    longValue = (longValue >> 63) ^ (longValue << 1);
    longValue = (longValue & 1) ? (~longValue | 1) : longValue;
	Long remaining = longValue;
	for (int i = 0; i < 10; ++i) {
		if ((remaining >> 7) != 0) {
			buffer[offset++] = (remaining | 0x80) & 0xFF;
		} else {
			buffer[offset++] = (remaining & 0x7f) & 0xFF;
			break;
		}

		remaining = remaining >> 7;
	}

    ZVAL_STRINGL(return_value, address(buffer[0]), offset);
}

PHP_METHOD(binary, readVarLong) {
    zval *value;
    zval* goffset;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &value, &goffset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    ZVAL_DEREF(goffset);
    char* buffer = Z_STRVAL_P(value);
    int offset = Z_LVAL_P(goffset);
    if (offset >= Z_STRLEN_P(value)) {
        zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
        ZVAL_NULL(return_value);
        return;
    }
	Long raw = 0;
	for (int i = 0; i <= 63; i += 7) {
		if (buffer[offset] == 0) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
            ZVAL_LONG(goffset, offset);
            ZVAL_NULL(return_value);
            return;
		}
		Long b = buffer[offset++];
		raw |= ((b & 0x7f) << i);

		if((b & 0x80) == 0){
            break;
		}
	}

	raw = (SignedLong)((raw & 1) ? ((raw << 63) ^ (~raw >> 1)) : ((raw << 63) ^ (raw >> 1)));
    ZVAL_LONG(goffset, offset);
    ZVAL_LONG(return_value, raw);
}

PHP_METHOD(binary, writeUnsignedVarLong) {
    char buffer[10];
    int offset = 0;
    Long longValue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &longValue) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	Long remaining = longValue;
	for (int i = 0; i < 10; ++i){
		if ((remaining >> 7) != 0) {
			buffer[offset++] = (remaining | 0x80) & 0xFF;
		} else {
			buffer[offset++] = (remaining & 0x7f) & 0xFF;
			break;
		}
		remaining = remaining >> 7;
	}
    ZVAL_STRINGL(return_value, address(buffer[0]), offset);
}

PHP_METHOD(binary, readUnsignedVarLong) {
    zval *b;
    zval* goffset;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &b, &goffset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    ZVAL_DEREF(goffset);
    char* buffer = Z_STRVAL_P(b);
    int offset = Z_LVAL_P(goffset);
    if (offset >= Z_STRLEN_P(b)) {
        zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
        ZVAL_NULL(return_value);
        return;
    }
	Long value = 0;
	for (int i = 0; i <= 63; i += 7) {
		if (buffer[offset] == 0) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 3);
            ZVAL_LONG(goffset, offset);
            ZVAL_NULL(return_value);
            return;
		}
		Long b = buffer[offset++];
		value |= ((b & 0x7f) << i);
		if((b & 0x80) == 0){
            break;
		}
	}
    ZVAL_LONG(goffset, offset);
    ZVAL_LONG(return_value, value);
}

PHP_METHOD(binary_stream, __construct) {
    zval* buffer;
    Long offset;
    binary_stream* stream = getThisStream();
    if (ZEND_NUM_ARGS() == 1) {
        if(zend_parse_parameters(ZEND_NUM_ARGS(), "z", &buffer) == FAILURE) {
            WRONG_PARAM_COUNT
        }
        stream->capacity = Z_STRLEN_P(buffer);
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->size = Z_STRLEN_P(buffer);
        stream->offset = 0;
        memcpy(stream->buffer, Z_STRVAL_P(buffer), Z_STRLEN_P(buffer));
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    } else if (ZEND_NUM_ARGS() == 2) {
        if(zend_parse_parameters(ZEND_NUM_ARGS(), "zl", &buffer, &offset) == FAILURE) {
            WRONG_PARAM_COUNT
        }
        stream->capacity = Z_STRLEN_P(buffer);
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->size = Z_STRLEN_P(buffer);
        stream->offset = offset;
        memcpy(stream->buffer, Z_STRVAL_P(buffer), Z_STRLEN_P(buffer));
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    } else if (ZEND_NUM_ARGS() > 2) {
        WRONG_PARAM_COUNT
    } else {
        stream->capacity = 64;
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->size = 0;
        stream->offset = 0;
    }
}

PHP_METHOD(binary_stream, rewind) {
    binary_stream* stream = getThisStream();
    stream->offset = 0;
    zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
}

PHP_METHOD(binary_stream, reset) {
    binary_stream* stream = getThisStream();
    stream->size = 0;
    stream->offset = 0;
    stream->capacity = 64;
    zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
}

PHP_METHOD(binary_stream, setOffset) {
    Long offset;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &offset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if (offset > stream->size) {
        stream->offset = stream->size;
    } else {
        stream->offset = offset;
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getOffset) {
    ZVAL_LONG(return_value, getThisStream()->offset);
}

PHP_METHOD(binary_stream, setBuffer) {
    zval* buffer;
    Long offset;
    binary_stream* stream = getThisStream();
    //efree(stream->buffer);
    if (ZEND_NUM_ARGS() == 1) {
        if(zend_parse_parameters(ZEND_NUM_ARGS(), "z", &buffer) == FAILURE) {
            WRONG_PARAM_COUNT
        }
        stream->capacity = Z_STRLEN_P(buffer);
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->size = Z_STRLEN_P(buffer);
        stream->offset = 0;
        memcpy(stream->buffer, Z_STRVAL_P(buffer), Z_STRLEN_P(buffer));
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    } else if (ZEND_NUM_ARGS() == 2) {
        if(zend_parse_parameters(ZEND_NUM_ARGS(), "zl", &buffer, &offset) == FAILURE) {
            WRONG_PARAM_COUNT
        }
        stream->capacity = Z_STRLEN_P(buffer);
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->size = Z_STRLEN_P(buffer);
        stream->offset = offset;
        memcpy(stream->buffer, Z_STRVAL_P(buffer), Z_STRLEN_P(buffer));
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    } else if (ZEND_NUM_ARGS() > 2) {
        WRONG_PARAM_COUNT
    } else {
        stream->capacity = 64;
        stream->buffer = (char*)emalloc(stream->capacity);
        stream->offset = 0;
        stream->size = 0;
    }
}

PHP_METHOD(binary_stream, getBuffer) {
    binary_stream* stream = getThisStream();
    ZVAL_STRINGL(return_value, stream->buffer, stream->size);
}

PHP_METHOD(binary_stream, get) {
    Long length;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &length) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < length) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_STRINGL(return_value, &stream->buffer[stream->offset], length);
        stream->offset += length;
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getRemaining) {
    binary_stream* stream = getThisStream();
    ZVAL_STRINGL(return_value, (char*)&stream->buffer[stream->offset], stream->size - stream->offset);
    stream->offset = stream->size;
    zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
}

PHP_METHOD(binary_stream, put) {
    zval *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, Z_STRLEN_P(value))) {
        memcpy(stream->buffer + stream->size, Z_STRVAL_P(value), Z_STRLEN_P(value));
        stream->size += Z_STRLEN_P(value);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getBool) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Bool)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_BOOL(return_value, readValue(stream, Bool));
        stream->offset += sizeof(Bool);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putBool) {
    Bool value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "b", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Bool))) {
        writeValue(stream, value);
        stream->size += sizeof(Bool);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getByte) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Byte)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, readValue(stream, Byte));
        stream->offset += sizeof(Byte);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putByte) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Byte value = (Byte)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Byte))) {
        writeValue(stream, value);
        stream->size += sizeof(Byte);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getShort) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Short value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Short));
        ZVAL_LONG(return_value, value);
        stream->offset += sizeof(Short);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getSignedShort) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(SignedShort)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        SignedShort value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(SignedShort));
        ZVAL_LONG(return_value, value);
        stream->offset += sizeof(SignedShort);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putShort) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Short value = (Short)argument; 
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Short))) {
        writeReverseValue(stream, value);
        stream->size += sizeof(Short);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLShort) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Short)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, readValue(stream, Short));
        stream->offset += sizeof(Short);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getSignedLShort) {
    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(SignedShort)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, readValue(stream, SignedShort));
        stream->offset += sizeof(SignedShort);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLShort) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Short value = (Short)argument;
    binary_stream* stream = asBinaryStream(getThis());
    if (updateCapacity(stream, sizeof(Short))) {
        writeValue(stream, value);
        stream->size += sizeof(Short);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getTriad) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < 3) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Triad value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], 3);
        ZVAL_LONG(return_value, value);
        stream->offset += 3;
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putTriad) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Triad value = (Triad)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, 3)) {
        writeReverseBytes(stream, address(value), 3);
        stream->size += 3;
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLTriad) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < 3) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Triad value = 0;
        memcpy(address(value), &stream->buffer[stream->offset], 3);
        ZVAL_LONG(return_value, value);
        stream->offset += 3;
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLTriad) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Triad value = (Triad)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Int))) {
        writeBytes(stream, address(value), 3);
        stream->size += 3;
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size); 
    }  
}

PHP_METHOD(binary_stream, getInt) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Int value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Int));
        ZVAL_LONG(return_value, value);
        stream->offset += sizeof(Int);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putInt) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Int value = (Int)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Int))) {
        writeReverseValue(stream, value);
        stream->size += sizeof(Int);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLInt) {
    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(Int)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, readValue(stream, Int));
        stream->offset += sizeof(Int);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLInt) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Int value = (Int)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Int))) {
        writeValue(stream, value);
        stream->size += sizeof(Int);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getFloat) {
    binary_stream* stream = getThisStream();    
    if (stream->size - stream->offset < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Float));
        ZVAL_DOUBLE(return_value, value);
        stream->offset += sizeof(Float);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getRoundedFloat) {
    Long accuracy;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &accuracy) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Float));
	    Float factor = pow(10.0, accuracy);
        ZVAL_DOUBLE(return_value, Long(value * factor) / factor);
        stream->offset += sizeof(Float);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    } 
}

PHP_METHOD(binary_stream, putFloat) {
    Double argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Float value = (Float)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Float))) {
        writeReverseValue(stream, value);
        stream->size += sizeof(Float);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLFloat) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_DOUBLE(return_value, readValue(stream, Float));
        stream->offset += sizeof(Float);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, getRoundedLFloat) {
    Long accuracy;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &accuracy) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Float)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Float value = readValue(stream, Float);
	    Float factor = pow(10.0, accuracy);
        ZVAL_DOUBLE(return_value, Long(value * factor) / factor);
        stream->offset += sizeof(Float);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }  
}

PHP_METHOD(binary_stream, putLFloat) {
    Double argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    Float value = (Float)argument;
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Float))) {
        writeValue(stream, value);
        stream->size += sizeof(Float);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getDouble) {
    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(Double)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Double value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Double));
        ZVAL_DOUBLE(return_value, value);
        stream->offset += sizeof(Double);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putDouble) {
    Double value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if (updateCapacity(stream, sizeof(Double))) {
        writeReverseValue(stream, value);
        stream->size += sizeof(Double);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLDouble) {
    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(Double)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_DOUBLE(return_value, readValue(stream, Double));
        stream->offset += sizeof(Double);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLDouble) {
    Double value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if(updateCapacity(stream, sizeof(Double))) {
        writeValue(stream, value);
        stream->size += sizeof(Double);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLong) {
    binary_stream* stream = getThisStream();
    if (stream->size - stream->offset < sizeof(Long)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        Long value = 0;
        reverseCopy(address(value), &stream->buffer[stream->offset], sizeof(Long));
        ZVAL_LONG(return_value, value);
        stream->offset += sizeof(Long);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLong) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();

    if(updateCapacity(stream, sizeof(Long))) {
        writeReverseValue(stream, value);
        stream->size += sizeof(Long);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getLLong) {
    binary_stream* stream = getThisStream();

    if (stream->size - stream->offset < sizeof(Long)) {
        zend_throw_exception(zend_exception_get_default(), "The requested length is less than the remainder.", 1);
        ZVAL_NULL(return_value);
    } else {
        ZVAL_LONG(return_value, readValue(stream, Long));
        stream->offset += sizeof(Long);
        zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
    }
}

PHP_METHOD(binary_stream, putLLong) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
    if(updateCapacity(stream, sizeof(Long))) {
        writeValue(stream, value);
        stream->size += sizeof(Long);
        zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
    }
}

PHP_METHOD(binary_stream, getUnsignedVarInt) {
    binary_stream* stream = getThisStream();
     Int value = 0;
	for (int i = 0; i <= 28; i += 7) {
		if (stream->offset >= stream->size) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 2);
            ZVAL_NULL(return_value);
		}
		Int b = stream->buffer[stream->offset++];
		value |= ((b & 0x7f) << i);

		if ((b & 0x80) == 0) {
            zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
            ZVAL_LONG(return_value, value);
			return;
		}
	}
	zend_throw_exception(zend_exception_get_default(), "VarInt did not terminate after 5 bytes!", 3);
    ZVAL_NULL(return_value);
}

PHP_METHOD(binary_stream, putUnsignedVarInt) {
    Long argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    binary_stream* stream = getThisStream();
	Int remaining = (Int)argument;
    if (updateCapacity(stream, 5)) {
        for (int i = 0; i < 5; ++i) {
            if ((remaining >> 7) != 0) {
                stream->buffer[stream->size++] = (remaining | 0x80) & 0xFF;
            } else {
                stream->buffer[stream->size++] = remaining & 0x7f;
                break;
            }
            remaining = remaining >> 7;
        }
    }
    zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
}

PHP_METHOD(binary_stream, getVarInt) {
    binary_stream* stream = getThisStream();

    Int value = 0;
	for (int i = 0; i <= 28; i += 7) {
		if (stream->offset >= stream->size) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 2);
            ZVAL_NULL(return_value);
		}
		Int b = stream->buffer[stream->offset++];
		value |= ((b & 0x7f) << i);

		if ((b & 0x80) == 0) { 
            zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
            ZVAL_LONG(return_value, (SignedInt)((value & 1) ? ((value << 31) ^ (~value >> 1)) : ((value << 31) ^ (value >> 1))));
			return;
		}
	}
	zend_throw_exception(zend_exception_get_default(), "VarInt did not terminate after 5 bytes!", 3);
    ZVAL_NULL(return_value);
}

PHP_METHOD(binary_stream, putVarInt) {
    SignedLong argument;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &argument) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    SignedInt svalue = (SignedInt)argument;
    Int value = (Int)svalue;
    value = (value << 1) ^ (value >> 31);
    value = (value & 1) ? (~value | 1) : value;
    binary_stream* stream = getThisStream();
	Int remaining = value;
    if(updateCapacity(stream, 5)) {
        for (int i = 0; i < 5; ++i){
            if ((remaining >> 7) != 0) {
                stream->buffer[stream->size++] = (remaining | 0x80) & 0xFF;
            } else {
                stream->buffer[stream->size++] = remaining & 0x7f;
                break;
            }

            remaining = remaining >> 7;
        }
    }
    zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
}

PHP_METHOD(binary_stream, getUnsignedVarLong) {
    binary_stream* stream = getThisStream();

    Long value = 0;
	for (int i = 0; i <= 63; i += 7) {
		if (stream->offset >= stream->size) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 2);
            ZVAL_NULL(return_value);
		}
		Long b = stream->buffer[stream->offset++];
		value |= ((b & 0x7f) << i);

		if((b & 0x80) == 0){
            zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
            ZVAL_LONG(return_value, value);
			return;
		}
	}
	zend_throw_exception(zend_exception_get_default(), "VarInt did not terminate after 5 bytes!", 3);
    ZVAL_NULL(return_value);
}

PHP_METHOD(binary_stream, putUnsignedVarLong) {
    Long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    binary_stream* stream = getThisStream();
	Long remaining = value;
    if(updateCapacity(stream, 10)) {
        for (int i = 0; i < 10; ++i) {
            if ((remaining >> 7) != 0) {
                stream->buffer[stream->size++] = (remaining | 0x80) & 0xFF;
            } else {
                stream->buffer[stream->size++] = remaining & 0x7f;
                break;
            }

            remaining = remaining >> 7;
        }
    }
    zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
}

PHP_METHOD(binary_stream, getVarLong) {
    binary_stream* stream = getThisStream();
    Long value = 0;
	for (int i = 0; i <= 63; i += 7) {
		if (stream->offset >= stream->size) {
			zend_throw_exception(zend_exception_get_default(), "No bytes left in buffer", 2);
            ZVAL_NULL(return_value);
		}
		Long b = stream->buffer[stream->offset++];
		value |= ((b & 0x7f) << i);

		if ((b & 0x80) == 0) {
            zend_update_property_long(binary_stream_ce, &stream->std, "offset", sizeof("offset") - 1, stream->offset);
            ZVAL_LONG(return_value, (SignedLong)((value & 1) ? ((value << 63) ^ (~value >> 1)) : ((value << 63) ^ (value >> 1))));
			return;
		}
	}
	zend_throw_exception(zend_exception_get_default(), "VarInt did not terminate after 5 bytes!", 3);
    ZVAL_NULL(return_value);
}

PHP_METHOD(binary_stream, putVarLong) {
    SignedLong lvalue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &lvalue) == FAILURE) {
        WRONG_PARAM_COUNT;
    }   
    Long value = (Long)lvalue;
    value = (value << 1) ^ (value >> 63);
    value = (value & 1) ? (~value | 1) : value;
    binary_stream* stream = getThisStream();
	Long remaining = value;
    if(updateCapacity(stream, 10)) {
        for (int i = 0; i < 10; ++i) {
            if ((remaining >> 7) != 0) {
                stream->buffer[stream->size++] = (remaining | 0x80) & 0xFF;
            } else {
                stream->buffer[stream->size++] = (remaining & 0x7f) & 0xFF;
                break;
            }

            remaining = remaining >> 7;
        }
    }
    zend_update_property_stringl(binary_stream_ce, &stream->std, "buffer", sizeof("buffer") - 1, stream->buffer, stream->size);
}

PHP_METHOD(binary_stream, feof) {
    binary_stream* stream = getThisStream();
    ZVAL_BOOL(return_value, stream->offset >= stream->size);
}

PHP_MINIT_FUNCTION(binary_utils) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Binary", binary_methods);
    binary_ce = zend_register_internal_class(&ce);
    binary_ce->create_object = binary_new;
    memcpy(&binary_handlers, zend_get_std_object_handlers(), sizeof(binary_handlers));
    binary_handlers.free_obj = binary_free;
    binary_handlers.dtor_obj = binary_destroy;
    binary_handlers.offset  = XtOffsetOf(binary, std);

    INIT_CLASS_ENTRY(ce, "BinaryStream", binary_stream_methods);
    binary_stream_ce = zend_register_internal_class(&ce);
    binary_stream_ce->create_object = binary_stream_new;
    memcpy(&binary_stream_handlers, zend_get_std_object_handlers(), sizeof(binary_stream_handlers));
    binary_stream_handlers.free_obj = binary_stream_free;
    binary_stream_handlers.dtor_obj = binary_stream_destroy;
    binary_stream_handlers.offset  = XtOffsetOf(binary_stream, std);
    zend_declare_property_string(binary_stream_ce, "buffer", sizeof("buffer") - 1, "", ZEND_ACC_PUBLIC);
    zend_declare_property_long(binary_stream_ce, "offset", sizeof("offset") - 1, 0, ZEND_ACC_PUBLIC);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(binary_utils) {
    return SUCCESS;
}

static zend_module_entry binary_utils_module_entry = { 
    STANDARD_MODULE_HEADER, "binary_utils", NULL,
    PHP_MINIT(binary_utils), NULL, NULL, NULL, NULL, 
    NO_VERSION_YET, STANDARD_MODULE_PROPERTIES 
};

ZEND_GET_MODULE(binary_utils)
