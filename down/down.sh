#!/usr/bin/env bash

#固定常量
PHONE_ROOT_DIR="/sdcard/Hdog/"       #手机存储根目录
LOCAL_ROOT_DIR="Hdog"                #本地存储根目录
LOCAL_FRAMEWORK="framework"          #本地框架目录
PHONE_FRAMEWORK="/system/framework/" #framework 文件还原odex

###########################################################
#                       check env
###########################################################
function check_env(){
    #adb
    command -v adb >/dev/null 2>&1 || { echo -e >&2 "\033[31m* Adb not installed. Aborting."; exit 1;}
}


###########################################################
#                       clean or make directory
#param1 dir
###########################################################
function clean_dir(){
    if [[ -d "$1" ]]; then
        rm -rf $1
    else
        mkdir $1
    fi
}


###########################################################
#                       decode odex
#param1 dir
#param2 framework dir
###########################################################
function decode_odex(){
    for package_dir in $1
    do
        if [[ -d ${package_dir} && (${package_dir} != "$1/framework") ]]; then
            echo "> >>Start decode $(basename ${package_dir}) odex"
            mkdir ${package_dir}/dey/bak

            for odex_dir in ${package_dir}/dey/*
            do
                if [[ -f ${odex_dir} ]]; then
                    base_odex=$(basename ${odex_dir})
                    dir_odex=$(dirname ${odex_dir})

                    echo "> Decode -> o-${base_odex%.*}.odex"
                    baksmali x ${odex_dir} -d $2 -o ${dir_odex}/smali
                    mv ${odex_dir} "${dir_odex}/bak/o-${base_odex%.*}.odex"
                    smali a ${dir_odex}/smali -o ${odex_dir%.*}.dex
                    rm -rf ${dir_odex}/smali

                fi
            done
        fi
    done
}


###########################################################
#                       main
#param1 args[]
###########################################################
function main(){
    check_env #check env

    clean_dir ${LOCAL_FRAMEWORK}
    adb pull ${PHONE_FRAMEWORK} . #down framework
    if [[ -n $@ ]]; then
        for arg in "$@"
        do
            dumped_path=${PHONE_ROOT_DIR}/${arg}
            clean_dir ${LOCAL_ROOT_DIR}/${arg} #clean local storage
            adb pull ${dumped_path} ${LOCAL_ROOT_DIR}
            echo "> Download dumped ${arg} -> ${LOCAL_ROOT_DIR}/${arg}"
            decode_odex ${LOCAL_ROOT_DIR}/${arg} "framework"
        done
    else
        clean_dir ${LOCAL_ROOT_DIR} #clean local storage
        adb pull ${PHONE_ROOT_DIR} .
        echo "> Download all dumped ${PHONE_ROOT_DIR} -> ${LOCAL_ROOT_DIR}"
        decode_odex ${LOCAL_ROOT_DIR}/* ${LOCAL_FRAMEWORK}
    fi
}

main "$@"