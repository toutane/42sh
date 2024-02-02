echo $(echo `echo $(echo Pierre), \`echo Paul, \` Jacques`) et Jean
echo $(echo `echo  \`echo Paul, \` $(echo Pierre), Jacques`) et Jean
echo `echo  \`echo Paul, \` Jacques` et Jean
echo $(echo `echo $(echo Pierre), $(echo Paul, ) Jacques`) et Jean
echo $(echo `echo $(echo Pierre), Jacques`) et Jean
echo $(echo `echo Jacques` Paul) et Jean
echo `echo $(echo Pierre), \`echo Paul, \` Jacques`
echo `echo $(echo Pierre) Jacques`
echo `echo \`echo Paul\``
echo $(echo \`echo Paul\`)
echo $(echo `echo  \`echo Paul, \` Jacques`) et Jean
