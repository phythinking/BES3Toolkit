#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# https://blog.csdn.net/wls666/article/details/100867234
# https://liushipeng.blog.csdn.net/article/details/91492491?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.baidujs&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.baidujs
class TextColor:
	"""Text color."""
	WHITE	     = '\033[30m'
	DEEPRED    = '\033[31m'
	GREEN      = '\033[32m'
	DEEPYELLOW = '\033[33m'
	BLUE       = '\033[34m'
	MAUVE      = '\033[35m' #淡紫色
	CYAN       = '\033[36m'
	GRAY       = '\033[37m'
	BLACK      = '\033[38m'
	HEADER     = '\033[95m' #magenta 洋红色
	WARNING    = '\033[93m' #yellow  黄色
	FAIL       = '\033[91m' #red	  红色  
	ENDC       = '\033[0m'
	BOLD       = '\033[1m'
	UNDERLINE  = '\033[4m'

	def white(text='Hello World'):
		"""Make white text"""
		return TextColor.WHITE + text + TextColor.ENDC

	def deepred(text='Hello World'):
		"""Make deepred text"""
		return TextColor.DEEPRED + text + TextColor.ENDC

	def green(text='Hello World'):
		"""Make green text"""
		return TextColor.GREEN + text + TextColor.ENDC

	def deepyellow(text='Hello World'):
		"""Make deepyellow text"""
		return TextColor.DEEPYELLOW + text + TextColor.ENDC

	def blue(text='Hello World'):
		"""Make blue text"""
		return TextColor.BLUE + text + TextColor.ENDC

	def mauve(text='Hello World'):
		"""Make mauve text"""
		return TextColor.MAUVE + text + TextColor.ENDC

	def cyan(text='Hello World'):
		"""Make cyan text"""
		return TextColor.CYAN + text + TextColor.ENDC

	def gray(text='Hello World'):
		"""Make gray text"""
		return TextColor.GRAY + text + TextColor.ENDC

	def black(text='Hello World'):
		"""Make black text"""
		return TextColor.BLACK + text + TextColor.ENDC

	def magenta(text='Hello World'):
		"""Make magenta text"""
		return TextColor.HEADER + text + TextColor.ENDC

	def yellow(text='Hello World'):
		"""Make yellow text"""
		return TextColor.WARNING + text + TextColor.ENDC

	def red(text='Hello World'):
		"""Make red text"""
		return TextColor.FAIL + text + TextColor.ENDC

	def bold(text='Hello World'):
		"""Make bold text"""
		return TextColor.BOLD + text + TextColor.ENDC

	def underline(text='Hello World'):
		"""Make underline text"""
		return TextColor.UNDERLINE + text + TextColor.ENDC

if __name__ == '__main__':
		  # Print color texts.
		  print(TextColor.white     ("字体颜色：白色"  ))
		  print(TextColor.deepred   ("字体颜色：深红色"))
		  print(TextColor.green     ("字体颜色：绿色"  ))
		  print(TextColor.deepyellow("字体颜色：黄色"  ))
		  print(TextColor.blue      ("字体颜色：蓝色"  ))
		  print(TextColor.mauve     ("字体颜色：淡紫色"))
		  print(TextColor.cyan      ("字体颜色：青色"  ))
		  print(TextColor.gray      ("字体颜色：灰色"  ))
		  print(TextColor.black     ("字体颜色：黑色"  ))
		  print(TextColor.magenta   ("字体颜色：洋红色"))
		  print(TextColor.yellow    ("字体颜色：黄色"  ))
		  print(TextColor.red       ("字体颜色：红色"  ))
		  print(TextColor.bold      ("字体粗体"        ))
		  print(TextColor.underline ("字体下划线"      ))
