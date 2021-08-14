/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Checked.h>
#include <base/TypeCasts.h>
#include <libjs/runtime/DataViewPrototype.h>

namespace JS {

DataViewPrototype::DataViewPrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void DataViewPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;

    define_native_function(vm.names.getBigInt64, get_big_int_64, 1, attr);
    define_native_function(vm.names.getBigUint64, get_big_uint_64, 1, attr);
    define_native_function(vm.names.getFloat32, get_float_32, 1, attr);
    define_native_function(vm.names.getFloat64, get_float_64, 1, attr);
    define_native_function(vm.names.getInt8, get_int_8, 1, attr);
    define_native_function(vm.names.getInt16, get_int_16, 1, attr);
    define_native_function(vm.names.getInt32, get_int_32, 1, attr);
    define_native_function(vm.names.getUint8, get_uint_8, 1, attr);
    define_native_function(vm.names.getUint16, get_uint_16, 1, attr);
    define_native_function(vm.names.getUint32, get_uint_32, 1, attr);
    define_native_function(vm.names.setBigInt64, set_big_int_64, 2, attr);
    define_native_function(vm.names.setBigUint64, set_big_uint_64, 2, attr);
    define_native_function(vm.names.setFloat32, set_float_32, 2, attr);
    define_native_function(vm.names.setFloat64, set_float_64, 2, attr);
    define_native_function(vm.names.setInt8, set_int_8, 2, attr);
    define_native_function(vm.names.setInt16, set_int_16, 2, attr);
    define_native_function(vm.names.setInt32, set_int_32, 2, attr);
    define_native_function(vm.names.setUint8, set_uint_8, 2, attr);
    define_native_function(vm.names.setUint16, set_uint_16, 2, attr);
    define_native_function(vm.names.setUint32, set_uint_32, 2, attr);

    define_native_accessor(vm.names.buffer, buffer_getter, {}, Attribute::Configurable);
    define_native_accessor(vm.names.byteLength, byte_length_getter, {}, Attribute::Configurable);
    define_native_accessor(vm.names.byteOffset, byte_offset_getter, {}, Attribute::Configurable);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(global_object.heap(), vm.names.DataView.as_string()), Attribute::Configurable);
}

DataViewPrototype::~DataViewPrototype()
{
}

static DataView* typed_this(VM& vm, GlobalObject& global_object)
{
    auto this_value = vm.this_value(global_object);
    if (!this_value.is_object() || !is<DataView>(this_value.as_object())) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, vm.names.DataView);
        return nullptr;
    }
    return static_cast<DataView*>(&this_value.as_object());
}

template<typename T>
static Value get_view_value(GlobalObject& global_object, Value request_index, Value is_little_endian)
{
    auto& vm = global_object.vm();
    auto* view = typed_this(vm, global_object);
    if (!view)
        return {};

    auto get_index = request_index.to_index(global_object);
    if (vm.exception())
        return {};
    auto little_endian = is_little_endian.to_boolean();

    auto buffer = view->viewed_array_buffer();
    if (buffer->is_detached()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::DetachedArrayBuffer);
        return {};
    }

    auto view_offset = view->byte_offset();
    auto view_size = view->byte_length();

    auto element_size = sizeof(T);

    Checked<size_t> buffer_index = get_index;
    buffer_index += view_offset;

    Checked<size_t> end_index = get_index;
    end_index += element_size;

    if (buffer_index.has_overflow() || end_index.has_overflow() || end_index.value() > view_size) {
        vm.throw_exception<RangeError>(global_object, ErrorType::DataViewOutOfRangeByteOffset, get_index, view_size);
        return {};
    }

    return buffer->get_value<T>(buffer_index.value(), false, ArrayBuffer::Order::Unordered, little_endian);
}

template<typename T>
static Value set_view_value(GlobalObject& global_object, Value request_index, Value is_little_endian, Value value)
{
    auto& vm = global_object.vm();
    auto* view = typed_this(vm, global_object);
    if (!view)
        return {};

    auto get_index = request_index.to_index(global_object);
    if (vm.exception())
        return {};

    Value number_value;
    if constexpr (IsIntegral<T> && sizeof(T) == 8)
        number_value = value.to_bigint(global_object);
    else
        number_value = value.to_number(global_object);
    if (vm.exception())
        return {};

    auto little_endian = is_little_endian.to_boolean();

    auto buffer = view->viewed_array_buffer();
    if (buffer->is_detached()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::DetachedArrayBuffer);
        return {};
    }

    auto view_offset = view->byte_offset();
    auto view_size = view->byte_length();

    auto element_size = sizeof(T);

    Checked<size_t> buffer_index = get_index;
    buffer_index += view_offset;

    Checked<size_t> end_index = get_index;
    end_index += element_size;

    if (buffer_index.has_overflow() || end_index.has_overflow() || end_index.value() > view_size) {
        vm.throw_exception<RangeError>(global_object, ErrorType::DataViewOutOfRangeByteOffset, get_index, view_size);
        return {};
    }

    return buffer->set_value<T>(buffer_index.value(), number_value, false, ArrayBuffer::Order::Unordered, little_endian);
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_big_int_64)
{
    return get_view_value<i64>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_big_uint_64)
{
    return get_view_value<u64>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_float_32)
{
    return get_view_value<float>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_float_64)
{
    return get_view_value<double>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_int_8)
{
    return get_view_value<i8>(global_object, vm.argument(0), Value(true));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_int_16)
{
    return get_view_value<i16>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_int_32)
{
    return get_view_value<i32>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_uint_8)
{
    return get_view_value<u8>(global_object, vm.argument(0), Value(true));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_uint_16)
{
    return get_view_value<u16>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::get_uint_32)
{
    return get_view_value<u32>(global_object, vm.argument(0), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_big_int_64)
{
    return set_view_value<i64>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_big_uint_64)
{
    return set_view_value<u64>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_float_32)
{
    return set_view_value<float>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_float_64)
{
    return set_view_value<double>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_int_8)
{
    return set_view_value<i8>(global_object, vm.argument(0), Value(true), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_int_16)
{
    return set_view_value<i16>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_int_32)
{
    return set_view_value<i32>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_uint_8)
{
    return set_view_value<u8>(global_object, vm.argument(0), Value(true), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_uint_16)
{
    return set_view_value<u16>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_FUNCTION(DataViewPrototype::set_uint_32)
{
    return set_view_value<u32>(global_object, vm.argument(0), vm.argument(2), vm.argument(1));
}

JS_DEFINE_NATIVE_GETTER(DataViewPrototype::buffer_getter)
{
    auto* data_view = typed_this(vm, global_object);
    if (!data_view)
        return {};
    return data_view->viewed_array_buffer();
}

JS_DEFINE_NATIVE_GETTER(DataViewPrototype::byte_length_getter)
{
    auto* data_view = typed_this(vm, global_object);
    if (!data_view)
        return {};
    if (data_view->viewed_array_buffer()->is_detached()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::DetachedArrayBuffer);
        return {};
    }
    return Value(data_view->byte_length());
}

JS_DEFINE_NATIVE_GETTER(DataViewPrototype::byte_offset_getter)
{
    auto* data_view = typed_this(vm, global_object);
    if (!data_view)
        return {};
    if (data_view->viewed_array_buffer()->is_detached()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::DetachedArrayBuffer);
        return {};
    }
    return Value(data_view->byte_offset());
}

}
