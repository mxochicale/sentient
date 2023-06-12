# References 

## Adding new literature
When adding a new manuscript or proceedings, it is suggested to do the following:
1. Copy and paste the template with -${year}, -${author}, -${venue of the publication} and -${PUBLISHER}:
``` 
cp -r 0000-YEAR-LastNameFirstAuthor-in-Journal-PUBLISHER/ ${year}-${author}-${venue of the publication}-${PUBLISHER}
```

2. Edit README and add the date when the work was retrieved, google citations, bibtex and other fields if there is information. See below one example:
```  
# 
## Citations
1

https://scholar.google.com/scholar?cites=15657830865269166932&as_sdt=2005&sciodt=0,5&hl=en
Retrived
Mon 20 Sep 19:50:40 BST 2021

## Authors 

## Notes

## Links 
https://github.com/laumerf/DeepHeartBeat

https://slideslive.com/38941017/deepheartbeat-latent-trajectory-learning-of-cardiac-cycles-using-cardiac-ultrasounds?locale=cs



## Bibtex 

@InProceedings{pmlr-v136-laumer20a,
  title = 	 {DeepHeartBeat: Latent trajectory learning of cardiac cycles using cardiac ultrasounds},
  author =       {Laumer, Fabian and Fringeli, Gabriel and Dubatovka, Alina and Manduchi, Laura and Buhmann, Joachim M.},
  booktitle = 	 {Proceedings of the Machine Learning for Health NeurIPS Workshop},
  pages = 	 {194--212},
  year = 	 {2020},
  editor = 	 {Alsentzer, Emily and McDermott, Matthew B. A. and Falck, Fabian and Sarkar, Suproteem K. and Roy, Subhrajit and Hyland, Stephanie L.},
  volume = 	 {136},
  series = 	 {Proceedings of Machine Learning Research},
  month = 	 {11 Dec},
  publisher =    {PMLR},
  pdf = 	 {http://proceedings.mlr.press/v136/laumer20a/laumer20a.pdf},
  url = 	 {https://proceedings.mlr.press/v136/laumer20a.html},
  abstract = 	 {Echocardiography monitors the heart movement for noninvasive diagnosis of heart diseases. It proves to be of profound practical importance as it combines low-cost portable instrumentation and rapid image acquisition without the risks of ionizing radiation. However, echocardiograms produce high-dimensional, noisy data which frequently proved difficult to interpret. As a solution, we propose a novel autoencoder-based framework, DeepHeartBeat, to learn human interpretable representations of cardiac cycles from cardiac ultrasound data. Our model encodes high dimensional observations by a cyclic trajectory in a lower dimensional space. We show that the learned parameters describing the latent trajectory are well interpretable and we demonstrate the versatility of our model by successfully applying it to various cardiologically relevant tasks, such as ejection fraction prediction and arrhythmia detection. As a result, DeepHeartBeat promises to serve as a valuable assistant tool for automating therapy decisions and guiding clinical care.}
}

```

3. Add bibtex file to the latex new-literature document
Open [new-literature.tex](new-literature.bib) and add bibtex of the paper. 
Then add a summary of the paper using citing the author in [../content-tex/new-literature.tex](../content-tex/new-literature.tex). 
For instance: `Laumer et al. proposed a novel autoencoder-based framework to learn human interpretable representation of cardiac cycles from cardiac ultrasound data \cite{laumer2020}`.

4. Commit changes to branch 10-new-literature 
```
git add .
git commit -m '#10 adds 2020-laumer-in-NeurIPS-PMLR; CITEX'
git push origin 10-new-literature  
```
