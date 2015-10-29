/*
##############################################################################
# UniSarasvati Devnagari Transliterator         Version 1.0 Beta                      
# Creator: Prasad Shirgaonkar                   prasad.shir@gmail.com                 
# Created 20/08/2004                            Last Modified 10/03/2007              
# This program comes on AS IS basis, without any warranty &/or support of any sort. 
# You are free to use and distribute this program freely! 
##############################################################################
# Revision history
# Revisied by: Atul Bhosale (atul.bhosale@gmail.com)
# Revisied on: 2/4/2011
*/

/* set or get input language */
(function ($) {
$(document).ready(function() {

  var inputLanguage = $('input:radio[name=language]:checked').val();
  function switchLanguage() {
    if('devnagari' == inputLanguage) {
      $('input:radio[value="roman"]').attr('checked', true);
      inputLanguage = 'roman';
    }
    else if('roman' == inputLanguage) {
      $('input:radio[value="devnagari"]').attr('checked', true);
      inputLanguage = 'devnagari';
    }
  }
  
  $('input:radio[name=language]').click(function(){
    switchLanguage();
  });
  
  $(document).keydown(function(e) {
    var key = e.charCode ? e.charCode : e.keyCode ? e.keyCode : 0;
    if(119 == key) {
      switchLanguage();
    }
      
    if('devnagari' == inputLanguage) {
      var target = e.target.id;     
      if('' != target) {  
        if( (key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123)) {
          setTimeout(function() { translateText(key, target); }, 5);
        }
      }
    }
  });
  
});


/* Treanslate the entered character */
function translateText(key, tar) {
  var txtLength = $("#" + tar).val().length;
  if(0 != txtLength) {
    previousChar = $("#" + tar).val().substring((txtLength - 2), (txtLength - 1));
    if(previousChar == chnbin) {
      previousChar = $("#" + tar).val().substring((txtLength - 3), (txtLength - 1));
      currentChar = $("#" + tar).val().substring((txtLength - 1), txtLength);
      prevString = $("#" + tar).val().substring(0, (txtLength - 3));
    }
    else {
      currentChar = $("#" + tar).val().substring((txtLength - 1), txtLength);
      prevString = $("#" + tar).val().substring(0, (txtLength - 2));
    }
    
    $("#" + tar).val('');
    $("#" + tar).val(prevString + getDevnagariCharacter(previousChar + currentChar));
  }
  else {
    previousChar = "";
    currentChar = $("#" + tar).val();
    $("#" + tar).val(getDevnagariCharacter(previousChar + currentChar));
  }
}


function getDevnagariCharacter(txt) {
  
  prvlen = txt.length;

  txt = txt.replace(/B/g, "b");
  txt = txt.replace(/C/g, "ch");
  txt = txt.replace(/F/gi, "ph");
  txt = txt.replace(/G/g, "g");
  txt = txt.replace(/J/g, "j");
  txt = txt.replace(/K/g, "k");
  txt = txt.replace(/P/g, "p");
  txt = txt.replace(/V/g, "v");
  txt = txt.replace(/W/gi, "v");
  txt = txt.replace(/X/g, "x");
  txt = txt.replace(/Z/gi, "jh");

  /* Actual Processing  phonetic string typing converts to halanta */
  for (i=0;i<input.length;i++) {
    txt = txt.replace(input[i], unescape(halanta[i]));
  }

  /* halanta followed by 'a' converts to vyanjana */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(halanta[i]+'a', unescape(vyanjan[i]));
  }

  /* halanta followed by 'space' converts to vyanjana */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(halanta[i]+' ', unescape(vyanjan[i]+' '));
  }

  /* halanta followed by punctuation mark converts to vyanjana with punctuation mark */
  for (i=0;i<halanta.length;i++) {
    for (k=0;k<punctuation_marks.length;k++) {
      txt = txt.replace(halanta[i]+punctuation_marks[k], unescape(vyanjan[i]+punctuation_marks[k]));
    }
  }

  /* kaanaa */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(vyanjan[i]+'a', unescape(vyanjan[i]+'ा'));
  }

  /* halanta followed by rhasva swar converts to vyanjana with rhasva swar */
  for (i=0;i<halanta.length;i++) {
    for (j=0;j<rhasva_swar.length;j++) {
      txt = txt.replace(halanta[i]+rhasva_swar_input[j], unescape(vyanjan[i]+rhasva_swar[j]));
    }
  }

  /* ikaarant to eekaarant */
  {txt = txt.replace('ि'+'i', unescape('ी'));}
  {txt = txt.replace('े'+'e', unescape('ी'));}

  /* ukaaraant to ookaarant */
  {txt = txt.replace('ु'+'u', unescape('ू'));}
  {txt = txt.replace('ो'+'o', unescape('ू'));}

  /* halanta followed by 'ai' converts to 'ai' kar */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(vyanjan[i]+'i', unescape(vyanjan[i]+'ै'));
  }

  /* okaar */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(halanta[i]+'o', unescape(vyanjan[i]+'ो'));
  }

  /* 'au' kaar */
  for (i=0;i<halanta.length;i++) {
    txt = txt.replace(vyanjan[i]+'u', unescape(vyanjan[i]+'ौ'));
  }
  
  /* logic for inputting swars as characters */
  for (i=0;i<all_swar.length;i++) {
    txt = txt.replace(all_swar_input[i], unescape(all_swar[i]));
  }

  txt = replaceEngNums(txt);
  sPos += (txt.length -prvlen +1);
  return txt;
}


function replaceEngNums(txt) {
  txt = txt.replace(/1/g, "\u0967");
  txt = txt.replace(/2/g, "\u0968");
  txt = txt.replace(/3/g, "\u0969");
  txt = txt.replace(/4/g, "\u096A");
  txt = txt.replace(/5/g, "\u096B");
  txt = txt.replace(/6/g, "\u096C");
  txt = txt.replace(/7/g, "\u096D");
  txt = txt.replace(/8/g, "\u096E");
  txt = txt.replace(/9/g, "\u096F");
  txt = txt.replace(/0/g, "\u0966");

  return txt;
}

var vyanjan = ['क','ख','ग','घ','ङ','च','छ','ज','झ','ञ','ट','ठ','ड','ढ','ण','त','थ','द','ध','न','प','फ','ब','भ','म','य','र','र','ल','ळ','व','स','श','ऽ','ष','ह','क्ष','ज्ञ','ॐ']
var halanta = ['क्','ख्','ग्','घ्','ङ्','च्','छ्','ज्','झ्','ञ्','ट्','ठ्','ड्','ढ्','ण्','त्','थ्','द्','ध्','न्','प्','फ्','ब्','भ्','म्','य्','र्','र्‍','ल्','ळ्','व्','स्','श्','ऽ','ष्','ह्','क्ष्', 'ज्ञ्','ॐ']
var input = ['k','क्h','g','ग्h','~N','ch','च्h','j','ज्h','Y','T','ट्h','D','ड्h','N','t','त्h','d','द्h','n','p','प्h','b','ब्h','m','y','r','R','l','L','v','s','स्h','S','ऽh','h','x','द्Y','ऑM']
var rhasva_swar = ['ि', 'ु', 'े', 'ृ' , 'ॅ' ,'ॅ' , 'ँ' , 'ं', 'ः','ा','ी','ू','ॉ']
var rhasva_swar_input = ['i', 'u', 'e','q', 'E','_', '^', 'M', 'H','A','I','U','O' ]
var all_swar = ['ँ', 'ं', 'ः', 'अ', 'आ', 'आ', 'इ', 'ई', 'ई', 'उ', 'ऊ', 'ऋ', 'ऌ', 'ए', 'ऐ', 'ऑ', 'ओ', 'औ','ऍ']
var all_swar_input = ['^','M', 'H','a','अअ','A','i','I','इइ','u','U','Q','Lu','e','अइ','O','o','अउ','E']
var punctuation_marks =['.','!','\,','\'','\"','\?','\;','\(','\)']
var chnbin = "\u094D";
var ugar = "\u0941";
var uugar = "\u0942";
var sPos = 0;
})(jQuery);