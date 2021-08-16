/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/temporal/AbstractOperations.h>
#include <libjs/runtime/temporal/Calendar.h>
#include <libjs/runtime/temporal/CalendarConstructor.h>
#include <libjs/runtime/temporal/PlainDate.h>
#include <libjs/runtime/temporal/PlainDateTime.h>
#include <libjs/runtime/temporal/PlainTime.h>
#include <libjs/runtime/temporal/PlainYearMonth.h>
#include <libjs/runtime/temporal/ZonedDateTime.h>
#include <libjs/runtime/Value.h>

namespace JS::Temporal {

Calendar::Calendar(String identifier, Object& prototype)
    : Object(prototype)
    , m_identifier(move(identifier))
{
}

Calendar* create_temporal_calendar(GlobalObject& global_object, String const& identifier, FunctionObject* new_target)
{
    auto& vm = global_object.vm();

    VERIFY(is_builtin_calendar(identifier));

    if (!new_target)
        new_target = global_object.temporal_calendar_constructor();

    auto* object = ordinary_create_from_constructor<Calendar>(global_object, *new_target, &GlobalObject::temporal_calendar_prototype, identifier);
    if (vm.exception())
        return {};

    return object;
}

bool is_builtin_calendar(String const& identifier)
{
    if (identifier != "iso8601"sv)
        return false;

    return true;
}

Calendar* get_builtin_calendar(GlobalObject& global_object, String const& identifier)
{
    auto& vm = global_object.vm();

    if (!is_builtin_calendar(identifier)) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendarIdentifier, identifier);
        return {};
    }

    // 2. Return ? Construct(%Temporal.Calendar%, « id »).
    MarkedValueList arguments(vm.heap());
    arguments.append(js_string(vm, identifier));
    auto calendar = vm.construct(*global_object.temporal_calendar_constructor(), *global_object.temporal_calendar_constructor(), move(arguments));
    if (vm.exception())
        return {};
    return static_cast<Calendar*>(&calendar.as_object());
}

Calendar* get_iso8601_calendar(GlobalObject& global_object)
{
    return get_builtin_calendar(global_object, "iso8601");
}

Vector<String> calendar_fields(GlobalObject& global_object, Object& calendar, Vector<StringView> const& field_names)
{
    auto& vm = global_object.vm();

    // 1. Let fields be ? GetMethod(calendar, "fields").
    auto fields = Value(&calendar).get_method(global_object, vm.names.fields);
    if (vm.exception())
        return {};

    // 2. Let fieldsArray be ! CreateArrayFromList(fieldNames).
    auto field_names_values = MarkedValueList { vm.heap() };
    for (auto& field_name : field_names)
        field_names_values.append(js_string(vm, field_name));
    Value fields_array = Array::create_from(global_object, field_names_values);

    // 3. If fields is not undefined, then
    if (fields) {
        // a. Set fieldsArray to ? Call(fields, calendar, « fieldsArray »).
        fields_array = vm.call(*fields, &calendar, fields_array);
        if (vm.exception())
            return {};
    }

    // 4. Return ? IterableToListOfType(fieldsArray, « String »).
    auto list = iterable_to_list_of_type(global_object, fields_array, { OptionType::String });
    if (vm.exception())
        return {};

    Vector<String> result;
    for (auto& value : list)
        result.append(value.as_string().string());
    return result;
}

double calendar_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Let result be ? Invoke(calendar, "year", « dateLike »).
    auto result = Value(&calendar).invoke(global_object, vm.names.year, &date_like);
    if (vm.exception())
        return {};

    if (result.is_undefined()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendarFunctionResult, vm.names.year.as_string());
        return {};
    }

    return result.to_integer_or_infinity(global_object);
}

double calendar_month(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Let result be ? Invoke(calendar, "month", « dateLike »).
    auto result = Value(&calendar).invoke(global_object, vm.names.month, &date_like);
    if (vm.exception())
        return {};

    // 3. If result is undefined, throw a RangeError exception.
    if (result.is_undefined()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendarFunctionResult, vm.names.month.as_string());
        return {};
    }

    // 4. Return ? ToPositiveIntegerOrInfinity(result).
    return to_positive_integer_or_infinity(global_object, result);
}

String calendar_month_code(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Let result be ? Invoke(calendar, "monthCode", « dateLike »).
    auto result = Value(&calendar).invoke(global_object, vm.names.monthCode, &date_like);
    if (vm.exception())
        return {};

    if (result.is_undefined()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendarFunctionResult, vm.names.monthCode.as_string());
        return {};
    }

    return result.to_string(global_object);
}

double calendar_day(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Let result be ? Invoke(calendar, "day", « dateLike »).
    auto result = Value(&calendar).invoke(global_object, vm.names.day, &date_like);
    if (vm.exception())
        return {};

    // 3. If result is undefined, throw a RangeError exception.
    if (result.is_undefined()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendarFunctionResult, vm.names.day.as_string());
        return {};
    }

    // 4. Return ? ToPositiveIntegerOrInfinity(result).
    return to_positive_integer_or_infinity(global_object, result);
}

Value calendar_day_of_week(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Return ? Invoke(calendar, "dayOfWeek", « dateLike »).
    return Value(&calendar).invoke(global_object, vm.names.dayOfWeek, &date_like);
}

Value calendar_day_of_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Return ? Invoke(calendar, "dayOfYear", « dateLike »).
    return Value(&calendar).invoke(global_object, vm.names.dayOfYear, &date_like);
}

Value calendar_week_of_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();

    return Value(&calendar).invoke(global_object, vm.names.weekOfYear, &date_like);
}

Value calendar_days_in_week(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    return Value(&calendar).invoke(global_object, vm.names.daysInWeek, &date_like);
}

Value calendar_days_in_month(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    return Value(&calendar).invoke(global_object, vm.names.daysInMonth, &date_like);
}

Value calendar_days_in_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    return Value(&calendar).invoke(global_object, vm.names.daysInYear, &date_like);
}

Value calendar_months_in_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    return Value(&calendar).invoke(global_object, vm.names.monthsInYear, &date_like);
}

Value calendar_in_leap_year(GlobalObject& global_object, Object& calendar, Object& date_like)
{
    auto& vm = global_object.vm();
    // 1. Assert: Type(calendar) is Object.

    // 2. Return ? Invoke(calendar, "inLeapYear", « dateLike »).
    return Value(&calendar).invoke(global_object, vm.names.inLeapYear, &date_like);
}

Object* to_temporal_calendar(GlobalObject& global_object, Value temporal_calendar_like)
{
    auto& vm = global_object.vm();

    if (temporal_calendar_like.is_object()) {
        auto& temporal_calendar_like_object = temporal_calendar_like.as_object();

        if (is<PlainDate>(temporal_calendar_like_object))
            return &static_cast<PlainDate&>(temporal_calendar_like_object).calendar();
        if (is<PlainDateTime>(temporal_calendar_like_object))
            return &static_cast<PlainDateTime&>(temporal_calendar_like_object).calendar();

        if (is<PlainTime>(temporal_calendar_like_object))
            return &static_cast<PlainTime&>(temporal_calendar_like_object).calendar();
        if (is<PlainYearMonth>(temporal_calendar_like_object))
            return &static_cast<PlainYearMonth&>(temporal_calendar_like_object).calendar();
        if (is<ZonedDateTime>(temporal_calendar_like_object))
            return &static_cast<ZonedDateTime&>(temporal_calendar_like_object).calendar();

        // b. If ? HasProperty(temporalCalendarLike, "calendar") is false, return temporalCalendarLike.
        auto has_property = temporal_calendar_like_object.has_property(vm.names.calendar);
        if (vm.exception())
            return {};
        if (!has_property)
            return &temporal_calendar_like_object;

        // c. Set temporalCalendarLike to ? Get(temporalCalendarLike, "calendar").
        temporal_calendar_like = temporal_calendar_like_object.get(vm.names.calendar);
        if (vm.exception())
            return {};
        // d. If Type(temporalCalendarLike) is Object and ? HasProperty(temporalCalendarLike, "calendar") is false, return temporalCalendarLike.
        if (temporal_calendar_like.is_object()) {
            has_property = temporal_calendar_like.as_object().has_property(vm.names.calendar);
            if (vm.exception())
                return {};
            if (!has_property)
                return &temporal_calendar_like.as_object();
        }
    }

    // 2. Let identifier be ? ToString(temporalCalendarLike).
    auto identifier = temporal_calendar_like.to_string(global_object);
    if (vm.exception())
        return {};

    // 3. If ! IsBuiltinCalendar(identifier) is false, then
    if (!is_builtin_calendar(identifier)) {
        // a. Let identifier be ? ParseTemporalCalendarString(identifier).
        auto parsed_identifier = parse_temporal_calendar_string(global_object, identifier);
        if (vm.exception())
            return {};
        identifier = move(*parsed_identifier);
    }

    // 4. Return ! CreateTemporalCalendar(identifier).
    return create_temporal_calendar(global_object, identifier);
}

Object* to_temporal_calendar_with_iso_default(GlobalObject& global_object, Value temporal_calendar_like)
{
    // 1. If temporalCalendarLike is undefined, then
    if (temporal_calendar_like.is_undefined()) {
        // a. Return ! GetISO8601Calendar().
        return get_iso8601_calendar(global_object);
    }
    // 2. Return ? ToTemporalCalendar(temporalCalendarLike).
    return to_temporal_calendar(global_object, temporal_calendar_like);
}

Object* get_temporal_calendar_with_iso_default(GlobalObject& global_object, Object& item)
{
    auto& vm = global_object.vm();

    if (is<PlainDate>(item))
        return &static_cast<PlainDate&>(item).calendar();
    if (is<PlainDateTime>(item))
        return &static_cast<PlainDateTime&>(item).calendar();

    if (is<PlainTime>(item))
        return &static_cast<PlainTime&>(item).calendar();
    if (is<PlainYearMonth>(item))
        return &static_cast<PlainYearMonth&>(item).calendar();
    if (is<ZonedDateTime>(item))
        return &static_cast<ZonedDateTime&>(item).calendar();

    // 2. Let calendar be ? Get(item, "calendar").
    auto calendar = item.get(vm.names.calendar);
    if (vm.exception())
        return {};

    // 3. Return ? ToTemporalCalendarWithISODefault(calendar).
    return to_temporal_calendar_with_iso_default(global_object, calendar);
}

PlainDate* date_from_fields(GlobalObject& global_object, Object& calendar, Object& fields, Object& options)
{
    auto& vm = global_object.vm();

    // 1. Assert: Type(calendar) is Object.
    // 2. Assert: Type(fields) is Object.

    // 3. Let date be ? Invoke(calendar, "dateFromFields", « fields, options »).
    auto date = Value(&calendar).invoke(global_object, vm.names.dateFromFields, &fields, &options);
    if (vm.exception())
        return {};

    // 4. Perform ? RequireInternalSlot(date, [[InitializedTemporalDate]]).
    auto* date_object = date.to_object(global_object);
    if (!date_object)
        return {};
    if (!is<PlainDate>(date_object)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Temporal.PlainDate");
        return {};
    }

    // 5. Return date.
    return static_cast<PlainDate*>(date_object);
}

bool calendar_equals(GlobalObject& global_object, Object& one, Object& two)
{
    auto& vm = global_object.vm();

    // 1. If one and two are the same Object value, return true.
    if (&one == &two)
        return true;

    // 2. Let calendarOne be ? ToString(one).
    auto calendar_one = Value(&one).to_string(global_object);
    if (vm.exception())
        return {};
    // 3. Let calendarTwo be ? ToString(two).
    auto calendar_two = Value(&two).to_string(global_object);
    if (vm.exception())
        return {};

    // 4. If calendarOne is calendarTwo, return true.
    if (calendar_one == calendar_two)
        return true;
    // 5. Return false.
    return false;
}

Object* consolidate_calendars(GlobalObject& global_object, Object& one, Object& two)
{
    auto& vm = global_object.vm();
    // 1. If one and two are the same Object value, return two.
    if (&one == &two)
        return &two;

    // 2. Let calendarOne be ? ToString(one).
    auto calendar_one = Value(&one).to_string(global_object);
    if (vm.exception())
        return {};

    // 3. Let calendarTwo be ? ToString(two).
    auto calendar_two = Value(&two).to_string(global_object);
    if (vm.exception())
        return {};

    // 4. If calendarOne is calendarTwo, return two.
    if (calendar_one == calendar_two)
        return &two;

    // 5. If calendarOne is "iso8601", return two.
    if (calendar_one == "iso8601"sv)
        return &two;

    // 6. If calendarTwo is "iso8601", return one.
    if (calendar_two == "iso8601"sv)
        return &one;

    // 7. Throw a RangeError exception.
    vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidCalendar);
    return {};
}

bool is_iso_leap_year(i32 year)
{
    if (year % 4 != 0)
        return false;

    if (year % 400 == 0)
        return true;

    if (year % 100 == 0)
        return false;

    return true;
}

u16 iso_days_in_year(i32 year)
{
    // 1. Assert: year is an integer.

    // 2. If ! IsISOLeapYear(year) is true, then
    if (is_iso_leap_year(year)) {
        // a. Return 366.
        return 366;
    }

    // 3. Return 365.
    return 365;
}

u8 iso_days_in_month(i32 year, u8 month)
{
    VERIFY(month >= 1 && month <= 12);

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return 31;

    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;

    // 5. If ! IsISOLeapYear(year) is true, return 29.
    if (is_iso_leap_year(year))
        return 29;

    // 6. Return 28.
    return 28;
}

u8 to_iso_day_of_week(i32 year, u8 month, u8 day)
{
    auto normalized_month = month + (month < 3 ? 10 : -2);
    auto normalized_year = year - (month < 3 ? 1 : 0);
    auto century = normalized_year / 100;
    auto truncated_year = normalized_year - (century * 100);
    auto result = (day + static_cast<u8>((2.6 * normalized_month) - 0.2) - (2 * century) + truncated_year + (truncated_year / 4) + (century / 4)) % 7;
    if (result <= 0) // Mathematical modulo
        result += 7;
    return result;
}

u16 to_iso_day_of_year(i32 year, u8 month, u8 day)
{
    u16 days = day;
    for (u8 i = month - 1; i > 0; --i)
        days += iso_days_in_month(year, i);
    return days;
}

u8 to_iso_week_of_year(i32 year, u8 month, u8 day)
{
    // 1. Assert: year is an integer.
    // 2. Assert: month is an integer.
    // 3. Assert: day is an integer.

    // 4. Let date be the date given by year, month, and day.
    // 5. Return date's week number according to ISO-8601.
    auto day_of_year = to_iso_day_of_year(year, month, day);
    auto day_of_week = to_iso_day_of_week(year, month, day);
    auto week = (day_of_year - day_of_week + 10) / 7;

    if (week < 1) {
        auto day_of_jump = to_iso_day_of_week(year, 1, 1);
        if (day_of_jump == 5 || (is_iso_leap_year(year) && day_of_jump == 6))
            return 53;
        else
            return 52;
    } else if (week == 53) {
        auto days_in_year = iso_days_in_year(year);
        if (days_in_year - day_of_year < 4 - day_of_week)
            return 1;
    }

    return week;
}

String build_iso_month_code(u8 month)
{
    return String::formatted("M{:02}", month);
}

double resolve_iso_month(GlobalObject& global_object, Object& fields)
{
    auto& vm = global_object.vm();

    // 1. Let month be ? Get(fields, "month").
    auto month = fields.get(vm.names.month);
    if (vm.exception())
        return {};

    // 2. Let monthCode be ? Get(fields, "monthCode").
    auto month_code = fields.get(vm.names.monthCode);
    if (vm.exception())
        return {};

    // 3. If monthCode is undefined, then
    if (month_code.is_undefined()) {
        // a. If month is undefined, throw a TypeError exception.
        if (month.is_undefined()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::TemporalMissingRequiredProperty, vm.names.month.as_string());
            return {};
        }
        // b. Return month.
        return month.as_double();
    }

    // 4. Assert: Type(monthCode) is String.
    VERIFY(month_code.is_string());
    auto& month_code_string = month_code.as_string().string();
    // 5. Let monthLength be the length of monthCode.
    auto month_length = month_code_string.length();
    // 6. If monthLength is not 3, throw a RangeError exception.
    if (month_length != 3) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidMonthCode);
        return {};
    }
    // 7. Let numberPart be the substring of monthCode from 1.
    auto number_part = month_code_string.substring(1);
    // 8. Set numberPart to ! ToIntegerOrInfinity(numberPart).
    auto number_part_integer = Value(js_string(vm, move(number_part))).to_integer_or_infinity(global_object);
    // 9. If numberPart < 1 or numberPart > 12, throw a RangeError exception.
    if (number_part_integer < 1 || number_part_integer > 12) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidMonthCode);
        return {};
    }
    // 10. If month is not undefined, and month ≠ numberPart, then
    if (!month.is_undefined() && month.as_double() != number_part_integer) {
        // a. Throw a RangeError exception.
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidMonthCode);
        return {};
    }
    // 11. If ! SameValueNonNumeric(monthCode, ! BuildISOMonthCode(numberPart)) is false, then
    if (month_code_string != build_iso_month_code(number_part_integer)) {
        // a. Throw a RangeError exception.
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidMonthCode);
        return {};
    }
    // 12. Return numberPart.
    return number_part_integer;
}

Optional<ISODate> iso_date_from_fields(GlobalObject& global_object, Object& fields, Object& options)
{
    auto& vm = global_object.vm();

    auto overflow = to_temporal_overflow(global_object, options);
    if (vm.exception())
        return {};

    // 3. Set fields to ? PrepareTemporalFields(fields, « "day", "month", "monthCode", "year" », «»).
    auto* prepared_fields = prepare_temporal_fields(global_object, fields, { "day", "month", "monthCode", "year" }, {});
    if (vm.exception())
        return {};

    // 4. Let year be ? Get(fields, "year").
    auto year = prepared_fields->get(vm.names.year);
    if (vm.exception())
        return {};

    // 5. If year is undefined, throw a TypeError exception.
    if (year.is_undefined()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::TemporalMissingRequiredProperty, vm.names.year.as_string());
        return {};
    }

    // 6. Let month be ? ResolveISOMonth(fields).
    auto month = resolve_iso_month(global_object, *prepared_fields);
    if (vm.exception())
        return {};

    // 7. Let day be ? Get(fields, "day").
    auto day = prepared_fields->get(vm.names.day);
    if (vm.exception())
        return {};

    // 8. If day is undefined, throw a TypeError exception.
    if (day.is_undefined()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::TemporalMissingRequiredProperty, vm.names.day.as_string());
        return {};
    }

    return regulate_iso_date(global_object, year.as_double(), month, day.as_double(), *overflow);
}

i32 iso_year(Object& temporal_object)
{
    if (is<PlainDate>(temporal_object))
        return static_cast<PlainDate&>(temporal_object).iso_year();
    if (is<PlainDateTime>(temporal_object))
        return static_cast<PlainDateTime&>(temporal_object).iso_year();
    if (is<PlainYearMonth>(temporal_object))
        return static_cast<PlainYearMonth&>(temporal_object).iso_year();
    VERIFY_NOT_REACHED();
}

u8 iso_month(Object& temporal_object)
{

    if (is<PlainDate>(temporal_object))
        return static_cast<PlainDate&>(temporal_object).iso_month();
    if (is<PlainDateTime>(temporal_object))
        return static_cast<PlainDateTime&>(temporal_object).iso_month();
    if (is<PlainYearMonth>(temporal_object))
        return static_cast<PlainYearMonth&>(temporal_object).iso_month();

    VERIFY_NOT_REACHED();
}

String iso_month_code(Object& temporal_object)
{
    if (is<PlainDate>(temporal_object))
        return build_iso_month_code(static_cast<PlainDate&>(temporal_object).iso_month());
    if (is<PlainDateTime>(temporal_object))
        return build_iso_month_code(static_cast<PlainDateTime&>(temporal_object).iso_month());
    if (is<PlainYearMonth>(temporal_object))
        return build_iso_month_code(static_cast<PlainYearMonth&>(temporal_object).iso_month());

    VERIFY_NOT_REACHED();
}

u8 iso_day(Object& temporal_object)
{
    if (is<PlainDate>(temporal_object))
        return static_cast<PlainDate&>(temporal_object).iso_day();
    if (is<PlainDateTime>(temporal_object))
        return static_cast<PlainDateTime&>(temporal_object).iso_day();
    if (is<PlainYearMonth>(temporal_object))
        return static_cast<PlainYearMonth&>(temporal_object).iso_day();

    VERIFY_NOT_REACHED();
}

}
