test01:;   echo OK

.RECIPEPREFIX=0345
test02:
0echo Now RecipePrefix is $(.RECIPEPREFIX)
.RECIPEPREFIX=

test03:
	echo default shell is $(SHELL)

test04:
	cd /tmp/
	touch a.txt

test05:
	cd /tmp/;echo $$$$
	touch a.txt;echo $$$$

#.ONESHELL:test05

test06:; echo 'hello \
	world' ; echo "hello \
    world"

test07:
	echo This is a phony target.
test08:FORCE
	echo test
FORCE:

.PHONY:test07 % This position is OK 

test09: test10 test10 test10 test11
	@echo $@
	@echo $<
	@echo $^
	@echo $+ 
test10 test11:
	@touch $@

test20:
ifeq ($(SHELL),/bin/sh)
	echo I am using sh
else
	echo I am using $(SHELL) instead of sh
endif
