## Google Cloud Instructions ##
**NOTE:** Before you start to use the google cloud to create your own virtual machine. You need to send an email to your TA (b.hu@yale.edu) to ask for a education promotion code to redeem $100 education credit.

1) sign up a google cloud account using your gmail account

2) redeem the education credit using the promotion code after your have signing up for the google cloud, click [here](https://urldefense.proofpoint.com/v2/url?u=https-3A__console.cloud.google.com_education&d=CwMFaQ&c=-dg2m7zWuuDZ0MUcV7Sdqw&r=6NctTORi3e2lyg8Rnp-_whgs9lQsWTrXeb1FitnDP30&m=lh8n9wzX33LBlmPU2jqUSZpkg_AaLuMLAbnvMCAZ4fg&s=UVSIP-GId86kE7jHy2oOqTnrgpF1Y3KNscMVBHiqaSM&e=)

3) create your own VM, [quick start guide](https://cloud.google.com/compute/docs/quickstart-linux)

the detailed specification of the VM is as follows:

![alt tag](https://git.yale.edu/CS426/Fall2016/blob/master/images/L1-vm-specification.png)

then, click the "Management, disk, networking, SSH keys" to add additional disk, which is used for Lab 2. Adding a disk using the following specification:

![alt tag](https://git.yale.edu/CS426/Fall2016/blob/master/images/L1-disk.png)

After creating a extra disk, you need to change the "When deleting instance" option from the default "keep disk" to "delete disk". After going through all the procedure mentioned above, your creating page will like the following one. And you click "create" button to create your own VM.

![alt tag](https://git.yale.edu/CS426/Fall2016/blob/master/images/L1-vm-final.png)

4) update your ssh public-private key pair, and ssh to your VM, click [here](https://cloud.google.com/compute/docs/instances/connecting-to-instance#generatesshkeypair) for detailed instructions
