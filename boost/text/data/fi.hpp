// Warning! This file is autogenerated.
#ifndef BOOST_TEXT_DATA_FI_HPP
#define BOOST_TEXT_DATA_FI_HPP

#include <boost/text/string_view.hpp>


namespace boost { namespace text { namespace data { namespace fi {

inline string_view search_collation_tailoring()
{
    return string_view(
u8R"(  
  
[normalization on]
[suppressContractions [เ-ไ ເ-ໄ ꪵ ꪶ ꪹ ꪻ ꪼ\\u19B5-\\u19B7\\u19BA]]
&'='<'≠'
&ا
  <<<ﺎ<<<ﺍ
  <<آ
  <<<ﺂ<<<ﺁ
  <<أ
  <<<ﺄ<<<ﺃ
  <<إ
  <<<ﺈ<<<ﺇ
&و
  <<<ۥ
  <<<ﻮ<<<ﻭ
  <<ؤ
  <<<ﺆ<<<ﺅ
&ي
  <<<ۦ
  <<<ﻳ<<<ﻴ<<<ﻲ<<<ﻱ
  <<ئ
  <<<ﺋ<<<ﺌ<<<ﺊ<<<ﺉ
  <<ى
  <<<ﯨ<<<ﯩ
  <<<ﻰ<<<ﻯ
&ه
  <<<ﻫ<<<ﻬ<<<ﻪ<<<ﻩ
  <<ة
  <<<ﺔ<<<ﺓ
&[last primary ignorable]<<׳
  <<״
  <<ـ
  <<ฺ
&ᄀ
  =ᆨ
&ᄀᄀ
  =ᄁ=ᆩ
&ᄀᄉ
  =ᆪ
&ᄂ
  =ᆫ
&ᄂᄌ
  =ᆬ
&ᄂᄒ
  =ᆭ
&ᄃ
  =ᆮ
&ᄃᄃ
  =ᄄ
&ᄅ
  =ᆯ
&ᄅᄀ
  =ᆰ
&ᄅᄆ
  =ᆱ
&ᄅᄇ
  =ᆲ
&ᄅᄉ
  =ᆳ
&ᄅᄐ
  =ᆴ
&ᄅᄑ
  =ᆵ
&ᄅᄒ
  =ᆶ
&ᄆ
  =ᆷ
&ᄇ
  =ᆸ
&ᄇᄇ
  =ᄈ
&ᄇᄉ
  =ᆹ
&ᄉ
  =ᆺ
&ᄉᄉ
  =ᄊ=ᆻ
&ᄋ
  =ᆼ
&ᄌ
  =ᆽ
&ᄌᄌ
  =ᄍ
&ᄎ
  =ᆾ
&ᄏ
  =ᆿ
&ᄐ
  =ᇀ
&ᄑ
  =ᇁ
&ᄒ
  =ᇂ
&ᅡᅵ
  =ᅢ
&ᅣᅵ
  =ᅤ
&ᅥᅵ
  =ᅦ
&ᅧᅵ
  =ᅨ
&ᅩᅡ
  =ᅪ
&ᅩᅡᅵ
  =ᅫ
&ᅩᅵ
  =ᅬ
&ᅮᅥ
  =ᅯ
&ᅮᅥᅵ
  =ᅰ
&ᅮᅵ
  =ᅱ
&ᅳᅵ
  =ᅴ
  
  
&D<<ð<<<Ð<<đ<<<Đ
&N<<ŋ<<<Ŋ
&T<<þ/h<<<Þ/h
&V<<w<<<W
&Y<<ü<<<Ü<<ű<<<Ű
&[before 1]ǀ<å<<<Å<ä<<<Ä<<æ<<<Æ<ö<<<Ö<<ø<<<Ø<<ő<<<Ő<<õ<<<Õ<<œ<<<Œ
  
  )");
}

inline string_view standard_collation_tailoring()
{
    return string_view(
u8R"(  
&D\\u0335<<đ<<<Đ
&G\\u0335<<ǥ<<<Ǥ
&N\\u0335<<ŋ<<<Ŋ
&T\\u0335<<ŧ<<<Ŧ
&Y<<ü<<<Ü
&Z\\u0335<<ʒ<<<Ʒ
&[before 1]ǀ<å<<<Å<ä<<<Ä<<æ<<<Æ<ö<<<Ö<<ø<<<Ø
  )");
}

inline string_view traditional_collation_tailoring()
{
    return string_view(
u8R"(  
&D<<ð<<<Ð<<đ<<<Đ
&N<<ŋ<<<Ŋ
&T<<þ/h<<<Þ/h
&V<<w<<<W
&Y<<ü<<<Ü<<ű<<<Ű
&[before 1]ǀ<å<<<Å<ä<<<Ä<<æ<<<Æ<ö<<<Ö<<ø<<<Ø<<ő<<<Ő<<õ<<<Õ<<œ<<<Œ
  )");
}


}}}}

#endif
