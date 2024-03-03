# Example models for timeseries 

## `timeseries_classification_from_scratch.ipynb`

epochs = 500 ## ORIGINAL
batch_size = 32 ##ORIGINAL


```
=================================================================
Total params: 25,858
Trainable params: 25,474
Non-trainable params: 384
```

```
Epoch 256/500
90/90 [==============================] - 1s 11ms/step - loss: 0.0521 - sparse_categorical_accuracy: 0.9837 - val_loss: 0.0865 - val_sparse_categorical_accuracy: 0.9722 - lr: 1.0000e-04
Epoch 256: early stopping
Execution time: 4.0526354789733885 minutes 
```

![fig](Screenshot%20from%202023-10-10%2002-20-17.png)


## `timeseries_transformer_classification.ipynb`

### head_size=256; num_heads=4; num_transformer_blocks = 2
```
==================================================================================================
Total params: 78,758
Trainable params: 78,758
Non-trainable params: 0
```

```
Epoch 77/200
360/360 [==============================] - 14s 38ms/step - loss: 0.0783 - sparse_categorical_accuracy: 0.9785 - val_loss: 0.4438 - val_sparse_categorical_accuracy: 0.8350
Execution time: 16.701254685719807 minutes 
```

![fig](Screenshot%20from%202023-10-10%2002-53-14.png)


### head_size=256; num_heads=4; num_transformer_blocks = 4 ##ORIGINAL

```
=================================================================
Total params: 93,130
Trainable params: 93,130
Non-trainable params: 0
```

```
Epoch 106/200
Execution time: 43.552278610070545 minutes 
```

![fig](Screenshot%20from%202023-10-10%2001-54-35.png)


### head_size=256; num_heads=4; num_transformer_blocks = 8

```
=================================================================
Total params: 121,874
Trainable params: 121,874
Non-trainable params: 0
```

```
Epoch 97/200
360/360 [==============================] - 48s 134ms/step - loss: 0.0737 - sparse_categorical_accuracy: 0.9747 - val_loss: 0.3841 - val_sparse_categorical_accuracy: 0.8530
Execution time: 78.16839495897293 minutes 
```

![fig](Screenshot%20from%202023-10-10%2001-54-43.png)


### head_size=512; num_heads=4; num_transformer_blocks = 4
```

=================================================================
Total params: 121,802
Trainable params: 121,802
Non-trainable params: 0
```

```
Epoch 84/200
360/360 [==============================] - 38s 107ms/step - loss: 0.0758 - sparse_categorical_accuracy: 0.9771 - val_loss: 0.4588 - val_sparse_categorical_accuracy: 0.8516
Execution time: 54.10847853024801 minutes 
```

![fig](Screenshot%20from%202023-10-10%2001-54-57.png)

