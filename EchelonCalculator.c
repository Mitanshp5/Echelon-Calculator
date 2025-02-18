#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define Near0 1e-6
#define S 20

int a, rrefform=0, refform=0, steps=0, rank=0, inverse=1;

//To print matrix
void printmat(float arr[S][S], int m, int n){
    printf("\n\n");
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if(fabs(arr[i][j])<Near0){
                printf("  0.00  ");
            } else if(arr[i][j]<0){
                printf(" %.2f  ", arr[i][j]);
            } else{
                printf("  %.2f  ", arr[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

//Taking input from user
void takeinput(float arr[S][S], int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            printf("Enter element %d %d: ", i+1, j+1);
            scanf("%f", &arr[i][j]);
        }
    }
}

//To perform row operations where a is the row who is changed, b is fixed
void ropt(float arr[S][S], int m, int n, int a , int b, int lead){
    float first = arr[a][lead];
        for(int i=lead; i<n; i++){
            arr[a][i]-= arr[b][i]*first;
        }
        if(steps==1){
            char o= (first>0) ? '-' : '+';
            printf("Row %d %c %.2f X Row %d\n", a+1, o, fabs(first), b+1);
            printmat(arr,m,n);
        }
}

//To swap rows a and b in a matrix
void swaprow(float arr[S][S], int m, int n,int a, int b){
    for(int i=0; i<m; i++){
        float temp = arr[a][i];
        arr[a][i] = arr[b][i];
        arr[b][i] = temp;
    }
    if(steps==1){
        printf("Row swap between Row %d and Row %d\n", a+1, b+1);
        printmat(arr,m,n);   
    }
}

//To convert first element to 1 in a row and lead is the column containing the first element
void convertto1(float arr[S][S], int m, int n, int a, int lead){
    float div= arr[a][lead];
    int did=0;
    for(int i=lead; i<n; i++){
        if(fabs(arr[a][i])>Near0){
            arr[a][i] /= div;
            did=1;
        }
    }
    if(steps==1 && did==1){
        printf("Division of row %d by %.2f\n", a+1, div);
        printmat(arr,m,n);
    }
}

//Ref
void ref(float arr[S][S], int m, int n){
    if(steps==1 && a!=3){
        printf("Original Matrix:\n");
        printmat(arr,m,n);
    }
    for(int i=0, lead=0, k=0; i<m && lead<n; i++, lead++){
        if(i==lead-k){
            if(abs(arr[i][lead])<Near0){
                int swap=0;
                for(int t=i+1; t<m; t++){
                    if (abs(arr[t][lead])>Near0){
                        swaprow(arr,m,n, i, t);
                        swap=1;
                        break;
                    }
                }
                if(swap==0){
                    k++;
                    i--;
                    continue;
                }
            }
            rank++;
            if(arr[i][lead]!=1){
                convertto1(arr,m,n, i, lead);
                refform=1;
            }
            for(int k=i+1;k<m;k++){
                if(abs(arr[k][lead])>Near0){
                    ropt(arr,m,n, k, i, lead);
                    refform=1;
                }
            }
        }
    }
}

//Rref
void rref(float arr[S][S], int m, int n){
    ref(arr,m,n);
    for(int i=0, lead=0; i<m && lead<n; i++, lead++){
        if(abs(arr[i][lead])<Near0){
            i--;
            inverse=0;
            continue;
        }
        for(int j=0;j<i;j++){
            if(abs(arr[j][lead])>Near0){
                ropt(arr,m,n, j, i, lead);
                rrefform=1;
            }
        }
    }
}

//Asking to show steps or not
void showsteps(){
    char s;
    printf("Do you want to see steps? (y/n): ");
    while(s!='y' && s!='n'){
        scanf(" %c",&s);
        s=tolower(s);
        if(s!='y' && s!='n'){
            printf("Please enter 'y' for yes or 'n' for no: ");
        } else if(s=='y'){
            steps=1;
            break;
        }
    }
}

//For inverse of matrix
void matInverse(float arr[S][S], int m){
    int n=2*m;
    
    //Taking input in augmented matrix
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(j<m){
                printf("Enter element %d %d: ",i+1, j+1);
                scanf("%f",&arr[i][j]);
            } else{
                arr[i][j] = (i==j-m)? 1:0;
            }
        }
    }
    if(steps==1){
        printf("Augmented matrix is:");
        printmat(arr,m,n);
    }
    rref(arr,m,n);

    //Printing the inverse of matrix if it exists
    if(inverse==1){
        printf("Inverse of the matrix is:");
        printf("\n\n");
        for(int i=0; i<m; i++){
            for(int j=m; j<n; j++){
                if(fabs(arr[i][j])<Near0){
                    printf("  0.00  ");
                } else if(arr[i][j]<0){
                    printf(" %.2f  ", arr[i][j]);
                } else{
                    printf("  %.2f  ", arr[i][j]);
                }
            }
            printf("\n");
        }
        printf("\n\n");
    } else{
        printf("Its a singular matrix, hence inverse does not exist.");
    }
}

//Unknown solver
void unkSolv(float arr[S][S], int m){
    int n=m+1;

    printf("Enter coefficients of the unknown variables (eg: []a1+[]a2....=[]):\n\n");
    //Taking input
    for(int i=0; i<m;i++){
        for(int j=0; j<n; j++){
            (j==m) ? printf("Enter the value: "): printf("Enter the coefficient of a%d in equation %d: ",j+1,i+1);
            scanf("%f", &arr[i][j]);
        }
    }
    rref(arr,m,n);

    //finding type of solution
    int type;
    if(arr[m-1][n-2]!=0){
        type=1;
    } else if(arr[m-1][n-2]==0 && arr[m-1][n-1]==0){
        type=2;
    } else{
        type=3;
    }

    //Showing ans
    switch(type){
        case 1: printf("\nThe solution of this system of equations:\n\n");
                for(int i=0; i<m; i++){
                    printf("a%d = %.2f\n", i+1, arr[i][m]);
                }
            break;
        case 2: printf("The given system of equations have infinitely many solutions.\n");
            break;
        case 3: printf("The given system of equations have no solutions.\n");
            break;
    }
}

void main();

//To continue or end program
void contOrEnd(){
    char ce;
    printf("\nDo you want to continue? (y/n): ");
    while(ce!='y' && ce!='n'){
        scanf(" %c",&ce);
        ce=tolower(ce);
        if(ce!='y' && ce!='n'){
            printf("Please enter 'y' for yes or 'n' for no: ");
        } else if(ce=='y'){
            rrefform=0, refform=0, steps=0, rank=0, inverse=1;
            printf("\n\n");
            main();
            break;
        }
    }
}

void main(){
    float arr[S][S];
    int m,n;
    printf("What u want to do?\n1) Find row echelon form\n2) Find reduced row echelon form\n3) Find inverse of a matrix\n4) Find unknown for a equation system\nEnter choice number: ");
    scanf("%d",&a);
    if(a==1||a==2){
        showsteps();
        printf("Enter the number of rows: ");
        scanf("%d",&m);
        printf("Enter the number of columns: ");
        scanf("%d",&n);
        takeinput(arr,m,n);
        if(a==1){
            ref(arr,m,n); 
            if(refform==1){
                printf("The row echelon form of the matrix is:\n");
                printmat(arr,m,n);
            } else{
                printf("\nThe matrix is already in row echelon form.\n");
            } 
        } else{
            rref(arr,m,n);
            if(rrefform==1){
                printf("The reduced row echelon form of the matrix is:");
                printmat(arr,m,n);
            } else{
                printf("\nThe given matrix can not be reduced.\n");
            }
        }
        printf("The rank of the given matrix is %d.", rank); 
    } else if(a==3){
        showsteps();
        printf("Enter the dimension of the square matrix whose inverse you want to find (eg: for 4x4 type \"4\", for 5x5 type \"5\"): ");
        scanf("%d",&m);
        matInverse(arr,m);
    } else if(a==4){
        printf("Enter number of unknowns (same number of equations are required):");
        scanf("%d", &m);
        unkSolv(arr,m);
    } else{
        printf("\nInvalid choice.\n");
    }
    contOrEnd();
}