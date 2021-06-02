#!/usr/bin/env python
# coding: utf-8

# ### Get Input File & Setup

# In[1]:


import csv

# table=[
#     [1,1,1,1,1],
#     [1,1,1,0,1],
#     [1,0,1,1,1],
#     [0,1,1,1,1],
#     [0,1,1,0,1],
#     [1,0,1,0,1],
#     [0,0,0,1,1],
#     [0,0,0,0,0]
# ]

# B = [
#     'A','B','C','D','E'
#     ]

NegFactorSet = {}

inputFile = open('input.csv')
csvInputFile = csv.reader(inputFile)

table = []
B = []

for index,row in enumerate(csvInputFile):
    del row[0]
    if not (index == 0):
        table.append([int(x) for x in row])
    else:
        B = row

print(B)
print(table)


# ### Perform Alg on File

# In[2]:


#STEP 0

# FactorTable is a hashtable/dictionary of all factors, factorA -> index | key -> value
# For everything in W, we will have to test this
# Over every factor, we are collecting the set of factors that are possible effects, this set is W
# B: set of all factors, main_table: C
def step0(main_table, B, NegFactorSet):
    W = []
    for index,factor in enumerate(B):
        if rowDuplicity(main_table, index):
            continue
        elif isNeg(index, NegFactorSet):
            continue
        W.append(index)
    return W


import copy
'''
    Check if the same row but negative factor already exists, if not, insert into set, else return 1
    int[i][j] table, i= coincidence, j = factor index
'''
def rowDuplicity(table, factor):
    rowSet = []
    #loop through every row in table
    for coincidence in table:
        modif_coincidence = copy.deepcopy(coincidence)
        modif_coincidence[factor] = 1-modif_coincidence[factor]
        if modif_coincidence in rowSet:
            return 1
        rowSet.append(coincidence)
        
def isNeg(factor, NegFactorSet):
    if factor in NegFactorSet:
        return 1
    return 0


# In[3]:


potential_effects = step0(table, B, NegFactorSet)


# ### Main Loop

# In[4]:


#-- HELPER FUNCTIONS - SELF SET FUNCTIONS --##

def setIn(fauxSet, coincidence):
    #loop through fauxSet
#     print("---------")
#     print(coincidence)
#     print(fauxSet)
#     print("---------")
    for elem in fauxSet:
        if len(elem) == len(coincidence):
#             print("888888")
#             print(elem)
#             print(coincidence)
#             print("888888")
            for index, fac in enumerate(elem):
                #compare each index in fauxElement to coincidence
                if coincidence[index] != fac:
                    break
                if index == len(elem)-1:
                    return True
    return False

def pairListsEqual(pairList1, pairList2):
    for pair1 in pairList1:
        exists = False
        for pair2 in pairList2:
            if pair1[0] == pair2[0] and pair1[1] == pair2[1]:
                exists = True
        if not exists:
            return False
    return True

def pairListIn(conditionList, pairList):
    for elem in conditionList:
        if pairListsEqual(elem, pairList):
            return True
    return False

#------------------------

def pairListListsEqual(pairListList1, pairListList2):
    if len(pairListList1) == len(pairListList2):
        for pairList in pairListList1:
            if not pairListIn(pairListList2, pairList):
                return False
        return True
    return False

def pairListListIn(conditionList, pairListList):
    for elem in conditionList:
        if pairListListsEqual(elem, pairListList):
            return True
    return False
        


# In[5]:



#-------------------------------------------------------------------------------
#STEP 1
#Just the loop that encases steps 2 and 3

#--------------------------------
#STEP 2

from numpy import *

'''
maint_table: the original table of values
effect: the factor being examined

conditionList: a list of all sufficient conditions
'''
def step2(main_table, effect):
    conditionList = []
    for coincidence in main_table:
        condition = []
        for index,value in enumerate(coincidence):
            if index != effect:
                condition.append((index,value))
            
        if check_sufficient(condition, main_table, effect):
            conditionList.append(condition)
    return conditionList
#--------------------------------
#STEP 3

from itertools import permutations
import copy
'''
maint_table: the original table of values
effect: the factor being examined
conditionList: a list of all sufficient conditions

minimally_sufficient_conditions: a set of all minimally sufficient conditions
'''
def step3(conditionList, main_table, effect):
    minimally_sufficient_conditions = []
    
    for condition in conditionList:
        prm_list = list(permutations(range(0,len(condition))))
        for prm in prm_list:
            #create pairlist of 
            modif_condition = []
            for index in prm:
                modif_condition.append(condition[index])
            tmp_list = copy.deepcopy(modif_condition)
            for factor in tmp_list:
                #remove
                modif_condition.remove(factor)
                #is sufficient?
                if not check_sufficient(modif_condition, table, effect):
                    modif_condition.append(factor)
            if not pairListIn(minimally_sufficient_conditions,modif_condition):
                minimally_sufficient_conditions.append(modif_condition)
                    
    return minimally_sufficient_conditions
                    
'''
Checks if a given list of pairs (a coincidence) is sufficient
'''
def check_sufficient(factorList, main_table, effect):
    foundOne = False
    for row in main_table:
        tmpvar = True
        for pair in factorList:
            if pair[1] != row[pair[0]]:
                tmpvar = False
                break
        if tmpvar:
            foundOne = True
            if row[effect] == 0:
                return 0
    return foundOne


# In[6]:


#--------------------------------
#STEP 4
#END OF FOR LOOP -- WE ARE IGNORING THIS STEP FOR EFFICIENCY

#--------------------------------
#STEP 5
'''
maint_table: the original table of values
effect: the factor being examined
sufficient_pair_lists: a list of all sufficient conditions 

pair structure -> (index, value)
'''
def step5(main_table, sufficient_pair_lists, effect):
    return check_necessary(main_table, sufficient_pair_lists, effect)

#--------------------------------
#STEP 6

from itertools import permutations

'''
maint_table: the original table of values
effect: the factor being examined
necessary_condition: the necessary condition (not minimized)

pair structure -> (index, value)

minimally_necessary_conditions: the conditions that are minimally necessary for the given effect
'''
def step6(main_table, necessary_condition, effect):
    minimally_necessary_conditions = []
    prm_list = list(permutations(range(0, len(necessary_condition))))
    for prm in prm_list:
        test_necessary_condition = []
        for index in prm:
            test_necessary_condition.append(necessary_condition[index])
        for i in range(0, len(test_necessary_condition)):
            suff_cond = test_necessary_condition.pop(0)
            if not check_necessary(main_table, test_necessary_condition, effect):
                test_necessary_condition.append(suff_cond)
        if not pairListListIn(minimally_necessary_conditions, test_necessary_condition):
            minimally_necessary_conditions.append(test_necessary_condition)
    return minimally_necessary_conditions

'''
maint_table: the original table of values
effect: the factor being examined
conditions: a list of minimally sufficient conditions made of pairs

pair structure -> (index, value)
'''   
def check_necessary(main_table, conditions, effect):
    for row in main_table:
        if row[effect] == 1:
            necessary = False
            for condition in conditions:
                found_match = True
                for pair in condition:
                    if not (pair[1] == row[pair[0]]):
                        found_match = False
                if found_match:
                    necessary = True
                    break
            if not necessary:
                return False
    return True


# In[7]:


#print(table)
for effect in potential_effects:
    print("_________________" + B[effect] + "____________________\n")
    
    print("ConditionList_________________")
    conditionList = step2(table,effect)
    for pairList in conditionList:
        print(pairList)
    print("end ConditionList_______________________\n")
    sufficientSet = step3(conditionList,table,effect)
    
    print("Sufficientset_________________")
    for pairList in sufficientSet:
        print(pairList)
        
    print("end Sufficientset_______________________\n")
        
#     if step5(table, sufficientSet, effect):
#         necessarySet = step6(table, sufficientSet, effect)
#         print("Necessaryset_________________")
#         for necessary in necessarySet:
#             print(necessary)
#         print("end Necessaryset_______________________\n")


# ### Framing

# In[ ]:




