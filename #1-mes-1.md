# logistic analysis

## business case;
+ When trying to distinguish between quality goods and defects based on the information obtained at the production site
+ Using the ROC concept to find the best model value as cut-off value

## procedure using R 
- dataset impoprt : x <- read_excel("source_data/data_train_1_3.xlsx", col_names = TRUE) 
  - raw data review : str(x) , summary(x)
  - Data cleaning operation depending on the data type : x$P_F <- as.factor(x$P_F )
 ___
- logistic regression :  m1 <- glm( P_F ~ x1 + x2 + x3 + x4 , data= x, family = binomial)
- Check the analyzed model : summary(m1)
- Check the analysis results : logit_f <- fitted( m1)
-- head(logit_f)
-- f1 <- round(logit_f )
- table( f1)

___
- data.frame 으로 비교
```
x1 <- as.data.frame( f1)
names(x1) <- c( "v1")
x1$v2 <- ifelse( x1$v1 >.5, 1,0)
is_correct <- ifelse( f1 >.5, 1,0) == as.numeric( x$P_F) # P_F --> 1 ~2
is_correct <- ifelse( f1 >.5, 1,0) == x$P_F 
table( is_correct)
sum(is_correct)/ NROW(is_correct)
 
# predict 
x1 <- c ( 85, 80, 90, 80, 81)
x2 <- c ( 85, 80, 90, 80, 80)
x3 <- c ( 75, 80, 90, 80, 80)
x4 <- c ( 60, 65, 70, 65, 66)

te <- as.data.frame( cbind(x1,x2,x3,x4))

str(te)
 
p1 <- predict( m1, newdata = te , type = "response") 

```

### Epi ROC 활용 
```
ibrary(Epi)
a1= ROC(form = P_F ~ x1 + x2 + x3 + x4 , data= x, plot="ROC")
a1
optimal_lr.eta=function(x){
  no=which.max(x$res$sens+x$res$spec)[1]
  result=x$res$lr.eta[no]
  result
}
optimal_lr.eta(a1) 

## cutoff value

optimal_cutpoint=function(x){
  y=optimal_lr.eta(x)
  b0=unname(x$lr$coeff[1])
  b1=unname(x$lr$coeff[2])
  result=(-log(1/y-1)-b0)/b1
  result
} 

optimal_cutpoint(a1) # 

```



