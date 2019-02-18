#!/bin/bash


readonly usage="\

Usage: 
	$0 [<platform>] 2>&1 | tee build.log
	$0

Examples:
	$0 ubuntu 2>&1 | tee build.log
	$0 renesas 2>&1 | tee build.log
	$0 z9 2>&1 | tee build.log
	$0 hw 2>&1 | tee build.log
	$0 all
	$0 clean
"

# 显示帮助
function show_help()
{
	echo -e "$usage"
}

function build_ubuntu()
{
	echo "build ubuntu::++++++++++"
	
	rm -rf ./build_ubuntu
	mkdir -p build_ubuntu

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	cd ./build_ubuntu
	cmake .. -DPLATFORM=ubuntu 
	make -j$core 
	
	echo "build ubuntu::----------"
}

function build_renesas()
{
	echo "build renesas::++++++++++"

	rm -rf ./build_renesas
	mkdir -p build_renesas
	
	unset LD_LIBRARY_PATH
	. /opt/poky/2.4.2/environment-setup-aarch64-poky-linux

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	cd ./build_renesas
	cmake .. -DPLATFORM=renesas
	make -j$core 
	
	echo "build renesas::----------"
}

function build_all()
{
	echo "build all::++++++++++"

	build_ubuntu
	build_renesas
	
	echo "build all::----------"
}

function build_clean()
{
	echo "build clean::++++++++++"

	rm -rf ./build_ubuntu
	rm -rf ./build_renesas
	
	echo "build clean::----------"
}

# 解析编译命令
function build_parse()
{
	echo "build parse::++++++++++"
	echo "platform = $1"

	case $1 in
		ubuntu)
			build_ubuntu
			;;
		renesas)
			build_renesas
			;;
		z9)
			build_z9
			;;
		all)
			build_all
			;;
		clean)
			build_clean
			;;
		*)
			echo "wrong platform: $i"
			show_help
			exit 1
			;;
	esac
	
	echo "build parse::----------"
}


set -x


# 获取顶层目录
TOP_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "TOP DIR = $TOP_DIR"

# 获取参数列表
echo "arg num = $#"
echo "args = $@"


if [ $# -eq 1 ]; then
	build_parse $1

	exit 0
fi

show_help
exit 1

