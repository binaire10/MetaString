#pragma once

#include <type_traits>
#include "Content.hpp"
#include "meta_Utility.hpp"
#include "meta_Serialize.hpp"
#include "meta_ContentUtility.hpp"

namespace meta
{
    template<char ch>
    using DigitToStr = Content<char, static_cast<char>('0'+ch)>;

    template<char ch>
    using DigitToHex = Content<char,
    (isInside<char, 0, 9>((0xf0&ch)>>4)?static_cast<char>(((0xf0&ch)>>4)+'0'):static_cast<char>(((0xf0&ch)>>4)-10+'a')),
    (isInside<char, 0, 9>(0xf&ch)?static_cast<char>((0xf&ch)+'0'):static_cast<char>((0xf&ch)-10+'a'))>;

    template<char ch>
    using DigitToBin = Content<char, static_cast<char>((0b1&ch)+'0')>;

    template<char ch>
    using DigitToOct = Content<char, static_cast<char>((0x7&ch)+'0')>;

    template<size_t byte>
    struct BinaryReader
    {
        template<unsigned long long number, template<char ch> class Encodeur, bool w = false>
        struct Reader_meta
        {
            typedef Serialize<typename Reader_meta<(number>>byte), Encodeur, true>::type, Encodeur<static_cast<char>(number)> > type;
        };

        template<template<char ch> class Encodeur, bool w>
        struct Reader_meta<0ll, Encodeur, w>
        {
            typedef typename std::conditional<w, Content<char>, Encodeur<0ull> >::type type;
        };
    };

    template<unsigned long long number, template<char ch> class Encodeur>
    using BinReader = typename BinaryReader<1>::Reader_meta<number, Encodeur>::type;

    template<unsigned long long number, template<char ch> class Encodeur>
    using OctetReader = typename BinaryReader<8>::Reader_meta<number, Encodeur>::type;

    template<unsigned long long number, template<char ch> class Encodeur>
    using OctalReader = typename BinaryReader<3>::Reader_meta<number, Encodeur>::type;

    template<unsigned long long number, template<char ch> class Encodeur, bool w = false>
    struct DizaineReader_meta
    {
        typedef Serialize<typename DizaineReader_meta<(number/10), Encodeur, true>::type, Encodeur<static_cast<char>(number%10)> > type;
    };

    template<template<char ch> class Encodeur, bool w>
    struct DizaineReader_meta<0ll, Encodeur, w>
    {
        typedef typename std::conditional<w, Content<char>, Encodeur<0ull> >::type type;
    };

    template<unsigned long long number, template<char ch> class Encodeur>
    using DizaineReader = typename DizaineReader_meta<number, Encodeur>::type;

    template<typename Type_t, unsigned s, Type_t value, typename Contents, long long int i = s-Contents::Size+1>
    struct FillContent_meta
    {
        typedef typename std::conditional<(i>0), Serialize<Content<Type_t, value>, typename FillContent_meta<Type_t, s, value, Contents,(i>0?i-1:0)>::fill>, Content<Type_t> >::type fill;
        typedef Serialize<fill, Contents> type;
    };

    template<typename Type_t, unsigned s, Type_t value, typename Contents>
    struct FillContent_meta<Type_t, s, value, Contents, 1>
    {
        typedef Content<Type_t> fill;
        typedef Serialize<fill, Contents> type;
    };

    template<typename Type_t, unsigned s, Type_t value, typename Contents>
    using FillContent = typename FillContent_meta<Type_t, s, value, Contents>::type;

    template<unsigned long long number>
    using HexToStr = OctetReader<number, DigitToHex>;

    template<unsigned long long number>
    using BinToStr = BinReader<number, DigitToBin>;

    template<unsigned long long number>
    using HexsFormatStr_ll = Serialize<Content<char, '0', 'x'>, FillContent<char, 16, '0', HexToStr<number> > >;

    template<unsigned long long number>
    using HexsFormatStr_l = Serialize<Content<char, '0', 'x'>, FillContent<char, 8, '0', HexToStr<number> > >;

    template<unsigned long long number>
    using HexsFormatStr_s = Serialize<Content<char, '0', 'x'>, FillContent<char, 4, '0', HexToStr<number> > >;

    template<unsigned long long number>
    using HexsFormatStr_c = Serialize<Content<char, '0', 'x'>, HexToStr<number> >;


    template<unsigned long long number>
    using BinsFormatStr_ll = Serialize<Content<char, '0', 'b'>, FillContent<char, 64, '0', BinToStr<number> > >;

    template<unsigned long long number>
    using BinsFormatStr_l = Serialize<Content<char, '0', 'b'>, FillContent<char, 32, '0',  BinToStr<number> > >;

    template<unsigned long long number>
    using BinsFormatStr_s = Serialize<Content<char, '0', 'b'>, FillContent<char, 16, '0', BinToStr<number> > >;

    template<unsigned long long number>
    using BinsFormatStr_c = Serialize<Content<char, '0', 'b'>, FillContent<char, 8, '0', BinToStr<number> > >;


    template<unsigned long long number>
    using DigitsFormatStr = DizaineReader<number, DigitToStr>;

    template<unsigned long long number>
    using BinsFormatStr = BinsFormatStr_ll<number>;

    template<unsigned long long number>
    using HexsFormatStr = HexsFormatStr_ll<number>;

    template<unsigned long long number>
    using OctsFormatStr = OctalReader<number, DigitToOct>;

    template<unsigned long long number, template<unsigned long long> class PrintFormat = DigitsFormatStr>
    using NumberToStr = PrintFormat<number>;

    static_assert(std::is_same<Content<char, '0', 'x',
                  '0', '0', '0', '0',
                  '0', '0', '0', '0',
                  '0', '0', '0', '0',
                  '0', '0', 'f', 'a'>, NumberToStr<0xfa, HexsFormatStr_ll> >::value, "test fail");
    static_assert(std::is_same<Content<char, '0', 'x',
                  '0', '0', '0', '0',
                  '0', '0', 'f', 'a'>, NumberToStr<0xfa, HexsFormatStr_l> >::value, "test fail");
    static_assert(std::is_same<Content<char, '0', 'x',
                  '0', '0', 'f', 'a'>, NumberToStr<0xfa, HexsFormatStr_s> >::value, "test fail");
    static_assert(std::is_same<Content<char, '0', 'x', 'f', 'a'>, NumberToStr<0xfa, HexsFormatStr_c> >::value, "test fail");
    static_assert(std::is_same<Content<char, '1', '0', '2', '4'>, NumberToStr<1024> >::value, "test fail");
    static_assert(std::is_same<Content<char, '2', '4'>, NumberToStr<24> >::value, "test fail");

    template<typename... Content_t>
    struct FormatArgument_meta
    {
        typedef Extract_end<Serialize<Content<char>, Serialize<Content_t, Content<char, ',', ' '> >... >, 2> type;
    };

    template<typename... etc>
    using FormatArgument = typename FormatArgument_meta<etc...>::type;
}
