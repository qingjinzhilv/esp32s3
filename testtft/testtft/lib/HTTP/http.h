#ifndef __HTTP_H__
#define __HTTP_H__
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
String Http_Get(const char *ip);
String Http_Post(const char *ip);

#endif
/* Code By Chengzhi233 
##############################
##############jjj#############
############jj  jj############
#######jjjjjj     jjjjj#######
######jj               #######
######j                j######
######j                j######
###jjj     tttt          jj###
###,      ttt              ###
###      tt    tttt        ###
###j    tt   tttttttt     ,###
###j    tt  ttt   tttt    j###
##jj   tt   tt      tt     j##
##j    tt  ttt   t  tt.     j#
#j     tt  ttt   tt  tt     j#
#j     tt   tt  ttt  tt     j#
#j     tt   tttttt   tt    j##
##j     tt   tttt   tt    jj##
##jj    ttt        ,tt    j###
###.     ttt      ttt      ###
###       ttttttttttt      j##
###j       tttttttt       jj##
###jjj        tt        jjj###
####jjj                jj#####
######j                j######
#######j              j#######
#######jjjjjj    jjjjjj#######
############jjjjjj############
##############jj##############
##############################
*/