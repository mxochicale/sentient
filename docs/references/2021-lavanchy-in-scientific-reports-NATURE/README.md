# Manuscript

## Citations
69 on Sat  7 Oct 17:59:22 BST 2023
https://scholar.google.com/scholar?cites=14668467219995507872&as_sdt=2005&sciodt=0,5&hl=en
[NUMBER_OF_CITATIONS]
[GOOGLE_CITATIONS_LINK]
[ACCESSED_DATE]


## Links 
https://www.nature.com/articles/s41598-021-84295-6

## Authors 

## Notes

"""
Calculation of motion features. Motion features calculated from the pre-processed instrument locations were
aimed to capture the characteristics of good/poor surgical skill. Skilled surgeons are known to handle instru-
ments in a narrow and focused area within their operative field. Poor surgical skill, on the other hand, is indi-
cated by slow, shaky movements with frequent direction changes and larger areas of motion.
"""

"""Modeling stage 3: skill prediction model. Data set and model training. The dataset consisted of ten
motion features calculated for each of 949 clipping video segments as well as the associated average skill rating.
Prior to training the skill prediction model, five out of the 949 clipping videos were removed due to showing
other surgical gestures. Most of clipping segments were rated by more than one expert therefore the average skill
rating was calculated.
A linear regression model was trained using the sklearn library 
based on the ten motion features as input and the average skill rating as the dependent variable.
"""

"""
As the third step, a linear regression model was trained to predict surgical skill based on the extracted motion
metrics. The contribution of each feature towards the prediction is shown in Fig. 3c with the ‘clipper count’ being
the most important. Predictions of the regression model were evaluated using accuracy 1/0 (binary, good vs.
poor surgical skill) and accuracy + 1/− 1 (skill level from 1 to 5, with ± 1 deviation). The linear regression model
achieved a performance of 87 ± 0.2% (mean ± SD) in accuracy 1/0 and 70 ± 0.2% in accuracy + 1/− 1.
"""

## bibtex 
```

```
