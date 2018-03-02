# glibc-mPvD
Modified GNU c library for mPvD support

# Patching glibc 2.27
* Simply run the script glibc.sh at the root of the repository:
  * chmod +x glibc.sh
  * ./glibc.sh

# Running tests on newly compiled glibc
* Go to the test folder:
  * cd tests

* Set all the .sh file executable:
  * chmod +x run_tests.sh
  * chmod +x sh-tests/*

* Before executing any tests, ensure pvdd is running:
  * pvdd -v 

* Compile and run the dns query code:
  * make
  * sudo ./run_tests.sh dns_query \<mode\> \<name\>
    * mode = 1 -> not using pvd feature
    * mode = 2 binding to a newly created pvd
    * mode = 3 using both of the above
    * name is the name that should be resolved
