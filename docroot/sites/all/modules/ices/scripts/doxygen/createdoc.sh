#!/bin/bash

dir_ices="$(dirname "$0")/../../"
doxygen_conf="scripts/doxygen/doxygen.conf"
extra_css="scripts/doxygen/extra.css"
doxygen_header="scripts/doxygen/header.html"
doxygen_footer="scripts/doxygen/footer.html"
doxygen_logo="scripts/doxygen/logo.png"
dir_actual="$(pwd)"


cd "$dir_ices"

if [ "$?" != 0 ]
then
  echo
  echo Error::Failed to get the directory module
  echo
  exit
fi

count=0
for d in docs/* ; do
  d=`basename $d`
  if [ "$d" != "img" ] ; then
    languages[$count]="$d"
  fi
  count=$(($count+1))
done

doxygen="/bin/doxygen"
log_file=/tmp/doxygen.log
tmp_conf=/tmp/doxygen.conf
debug=0

## List of errors with vim format
errors_log=/tmp/doxygen.errors

function cd_help() {

  echo
  echo 'Usage: createdoc [debug]'
  echo
  echo 'This script uses the configuration file doxygen.conf to generate documentation.'
  echo
  echo Detected languages are: ${languages[*]}
  echo
  echo To see debug messages add "debug" as the first parameter.
  echo
  echo If we find doxygen.css, header.html or footer.html were applied to run the script
  echo

}

if [ "$1" == "debug" ]
then
  debug=1
  shift 1
fi

if [ "$1" == "help" ] || \
   [ "$1" == "-help" ] || \
   [ "$1" == "-h" ]
   
then 
  cd_help
  exit
fi

if [ $debug == 1 ] ; then
  echo
  echo Path doxygen: $doxygen
  echo File config: $doxygen_conf
  echo Log file: $log_file 
  echo Erros log: $errors_log
  echo Dir ices: $dir_ices
  echo Dir actual: $(pwd)
  echo
fi

for lang in ${languages[*]} ; do

  if [ "$lang" == "img" ] ; then
    break
  fi

  # Define language in long format
  #
  # The default language is English, other supported languages are: 
  # Afrikaans, Arabic, Brazilian, Catalan, Chinese, Chinese-Traditional, 
  # Croatian, Czech, Danish, Dutch, Esperanto, Farsi, Finnish, French, German, 
  # Greek, Hungarian, Italian, Japanese, Japanese-en (Japanese with English 
  # messages), Korean, Korean-en, Lithuanian, Norwegian, Macedonian, Persian, 
  # Polish, Portuguese, Romanian, Russian, Serbian, Serbian-Cyrilic, Slovak, 
  # Slovene, Spanish, Swedish, Ukrainian, and Vietnamese.
  # The default language is English, other supported languages are: 
  # Afrikaans, Arabic, Brazilian, Catalan, Chinese, Chinese-Traditional, 
  # Croatian, Czech, Danish, Dutch, Esperanto, Farsi, Finnish, French, German, 
  # Greek, Hungarian, Italian, Japanese, Japanese-en (Japanese with English 
  # messages), Korean, Korean-en, Lithuanian, Norwegian, Macedonian, Persian, 
  # Polish, Portuguese, Romanian, Russian, Serbian, Serbian-Cyrilic, Slovak, 
  # Slovene, Spanish, Swedish, Ukrainian, and Vietnamese.

  case $lang in
    es)
      lang_long=Spanish;;
    en)
      lang_long=English;;
    ca)
      lang_long=Catalan;;
    fr)
      lang_long=French;;
    de)
      lang_long=German;;
    *)
      lang_long=English;;
  esac

  # Create documentation directory according to language

  if [ $debug == 1 ] ; then
    echo
    echo Lang: $lang / $lang_long
    echo
  fi

  [[ -d "docs/tmp" ]] && rm -fr "docs/tmp"
  mkdir "docs/tmp"
  cp -R docs/es/* docs/tmp/

  if [ "$lang" != "es" ]
  then
    cp -R docs/$lang/* docs/tmp/
  fi

  # Generate final file for doxygen

  dir_final="$(grep ^OUTPUT_DIRECTORY $doxygen_conf | cut -d= -f 2 | sed 's/^ //')"
  html_output="$(grep ^HTML_OUTPUT $doxygen_conf | cut -d= -f 2 | sed 's/^ //')"
  log_file="$(grep ^WARN_LOGFILE $doxygen_conf | cut -d= -f 2 | sed 's/^ //')"

  cp "$doxygen_conf" "$tmp_conf"
  echo -e "\n\nOUTPUT_LANGUAGE=$lang_long" >> $tmp_conf
  echo -e "\n\nHTML_OUTPUT=$lang" >> $tmp_conf

  if [ -f "$extra_css" ] ; then
    [[ $debug == 1 ]] && echo -e "\nAdd extra css: `pwd`/$extra_css ."
    echo -e "\n\nHTML_EXTRA_STYLESHEET=`pwd`/$extra_css" >> $tmp_conf
  fi

  if [ -f "$doxygen_header" ] ; then
    [[ $debug == 1 ]] && echo -e "\nAdd header: $doxygen_header"
    echo -e "\n\nHTML_HEADER=$doxygen_header" >> $tmp_conf
  fi

  if [ -f "$doxygen_footer" ] ; then
    [[ $debug == 1 ]] && echo -e "\nAdd footer: $doxygen_footer"
    echo -e "\n\nHTML_FOOTER=$doxygen_footer" >> $tmp_conf
  fi

  if [ -f "$doxygen_logo" ] ; then
    [[ $debug == 1 ]] && echo -e "\nAdd logo: $doxygen_logo"
    echo -e "\n\nPROJECT_LOGO=`pwd`/$doxygen_logo" >> $tmp_conf
    echo -e "\n\nPROJECT_NAME=" >> $tmp_conf
  fi

  # Delete final dir 
  if [ -d "${dir_final}${lang}" ]
  then
    if [ $debug == 1 ]
    then
      echo
      echo Delete ${dir_final}${lang} first.
      echo
    fi
    rm -fr "${dir_final}${lang}"
  fi

  # Run doxygen
  if [ $debug == 1 ]
  then
    echo
    echo Run doxygen.
    echo
    cat $tmp_conf | $doxygen - 2> $errors_log
  else
    cat $tmp_conf | $doxygen - > /tmp/salida.dox 2> $errors_log
  fi

  if [ $debug == 1 ] ; then
    echo
    echo Final dir: ${dir_final}${lang}
    echo Output language: $lang_long
    echo Log file: $log_file
    echo 
    echo -e "$conf"
  fi

  cd "$dir_actual"

  if [ $debug == 1 ] ; then
    echo
    read -p "Abrir gvim con errores de doxygen (s/n): " OPCION
    echo
    if [ "$OPCION" == 's' ] ; then
      gvim -c "cfile $log_file" 
      read
    fi
  fi


  ## Delete temporal files
  f="docs/tmp" ; [[ -d "$f" ]] && rm -fr "$f"
  f="$tmp_conf" ; [[ -d "$f" ]] && rm -fr "$f"

done

