#!/bin/bash


readonly usage="\

Usage: 
	$0 [<platform>] 2>&1 | tee build.log
	$0

Examples:
	$0 ubuntu 2>&1 | tee build.log
	$0 renesas 2>&1 | tee build.log
	$0 xilinx 2>&1 | tee build.log
	$0 apps
	$0 uapps
	$0 rapps
	$0 xapps
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
	
	rm -rf $TOP_DIR/build_ubuntu
	mkdir -p $TOP_DIR/build_ubuntu

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	cd $TOP_DIR/build_ubuntu
	cmake .. -DPLATFORM=ubuntu 
	make -j$core 
	
	echo "build ubuntu::----------"
}

function build_renesas()
{
	echo "build renesas::++++++++++"

	rm -rf $TOP_DIR/build_renesas
	mkdir -p $TOP_DIR/build_renesas
	
	unset LD_LIBRARY_PATH
	. /opt/poky/2.4.2/environment-setup-aarch64-poky-linux

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	cd $TOP_DIR/build_renesas
	cmake .. -DPLATFORM=renesas
	make -j$core 
	
	echo "build renesas::----------"
}

function build_xilinx()
{
	echo "build xilinx::++++++++++"

	rm -rf $TOP_DIR/build_xilinx
	mkdir -p $TOP_DIR/build_xilinx
	
	unset LD_LIBRARY_PATH
	. /opt/petalinux/2018.3/environment-setup-aarch64-xilinx-linux

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	cd $TOP_DIR/build_xilinx
	cmake .. -DPLATFORM=xilinx
	make -j$core 
	
	echo "build xilinx::----------"
}

function build_all()
{
	echo "build all::++++++++++"

	build_ubuntu
	build_renesas
	build_xilinx

	echo "build all::----------"
}

function build_clean()
{
	echo "build clean::++++++++++"

	rm -rf $TOP_DIR/build_ubuntu
	rm -rf $TOP_DIR/build_renesas
	rm -rf $TOP_DIR/build_xilinx
	
	echo "build clean::----------"
}


function build_apps()
{
	echo "build apps::++++++++++"

	cd $1
	rm -rf $1/build_$2
	mkdir -p $1/build_$2
	
	cd $1/build_$2
	cmake .. -DPLATFORM=$2 

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	make -j$core

	echo "build apps::-----------"
}


function build_ubuntu_apps()
{
	echo "build ubuntu apps::++++++++++"

	cd $TOP_DIR/apps	

	for file in `ls $1` 
	do 
  		echo $TOP_DIR/apps"/"$file 
  		
		if [ -d $TOP_DIR/apps"/"$file ] 
  		then 
   			echo $TOP_DIR/apps"/"$file
			build_apps $TOP_DIR/apps"/"$file ubuntu
  		fi
 	done

	echo "build ubuntu apps::-----------" 
}

function build_renesas_apps()
{
	echo "build renesas apps::++++++++++"

	unset LD_LIBRARY_PATH
	. /opt/poky/2.4.2/environment-setup-aarch64-poky-linux


	cd $TOP_DIR/apps	

	for file in `ls $1` 
	do 
  		echo $TOP_DIR/apps"/"$file 
  		
		if [ -d $TOP_DIR/apps"/"$file ] 
  		then 
   			echo $TOP_DIR/apps"/"$file
			build_apps $TOP_DIR/apps"/"$file renesas
  		fi
 	done

	echo "build renesas apps::-----------" 
}

function build_xilinx_apps()
{
	echo "build xilinx apps::++++++++++"

	unset LD_LIBRARY_PATH
	. /opt/petalinux/2018.3/environment-setup-aarch64-xilinx-linux

	cd $TOP_DIR/apps	

	for file in `ls $1` 
	do 
  		echo $TOP_DIR/apps"/"$file 
  		
		if [ -d $TOP_DIR/apps"/"$file ] 
  		then 
   			echo $TOP_DIR/apps"/"$file
			build_apps $TOP_DIR/apps"/"$file xilinx
  		fi
 	done

	echo "build xilinx apps::-----------" 
}

# 解析编译命令
function build_parse()
{
	echo "build parse::++++++++++"
	echo "platform = $1"

	case $1 in
		ubuntu)
			build_ubuntu
			#build_ubuntu_apps
			;;
		renesas)
			build_renesas
			build_renesas_apps
			;;
		xilinx)
			build_xilinx
			build_xilinx_apps
			;;
		apps)
			build_ubuntu_apps
			build_renesas_apps
			build_xilinx_apps
			;;
		uapps)
			build_ubuntu_apps
			;;
		rapps)
			build_renesas_apps
			;;
		xapps)
			build_xilinx_apps
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

