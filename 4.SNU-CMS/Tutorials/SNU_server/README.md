# SNU Server User Guide
#### Table of Contents
[SNU servers](#snu-servers)  
[Access](#access)  
[Storage](#storage)  
[Job scheduler *condor*](#job-scheduler-condor)  
[Advanced uasge](#advanced-usage)  
[Additional notes for CMS users](#additional-notes-for-cms-users)  
[Troubleshooting](#troubleshooting)  


## SNU servers
### Cluster servers
The cluster servers consist of two submission machines and 19 working machines. You can login to submission machines and submit your jobs with *condor*, then *condor* will distribute your jobs over working machines. **Do not run heavy jobs directly at submission machines**. Instead, please use *condor* scheduler or [standalone servers](#standalone-servers).

**tamsa1.snu.ac.kr**: Submission machine, Centos7, 16 threads, RAM 40G  
**tamsa2.snu.ac.kr**: Submission machine, Centos7, 8 threads, RAM 16G  
**n[1-11], node[1-11]**: Working machines, Centos7, 20~48 threads/machine (total 948 threads), RAM ~ 4G/thread, two RTX3090  

### Standalone servers
The standalone servers are just computers with many threads. You can run job directly without job scheduling.  
**cms1.snu.ac.kr**: SLC6, 72 threads, RAM 256G  
**cms2.snu.ac.kr**: Centos7, 72 threads, RAM 256G  

## Access
All SNU servers use *ssh* port 1240 instead of 22
```
ssh -p 1240 USERNAME@tamsa1.snu.ac.kr
# add -Y option for graphics
```
### *ssh* configuration (*optional*)
You can configure the default *ssh* settings with the file *~/.ssh/config* for easier access.
```
# example of ~/.ssh/config
Host tamsa1
  HostName tamsa1.snu.ac.kr
  User USERNAME #replace USERNAME with your account name
  Port 1240
  ForwardX11 Yes #same with 'ssh -Y', for graphics
```
You can just run `ssh tamsa1` with this configuration file.

### Access with a rsa key (*optional*)
You can use the rsa key method for the authentication rather than using a password.
```
### client side
# Generate rsa key pair. If you have already, please skip.
ssh-keygen -t rsa -f ~/.ssh/id_rsa -q -N ""
```
The code above will generate two files, *id_rsa* and *id_rsa.pub* in *~/.ssh/*. The *id_rsa* is the private key which **should not be public** to others. The *id_rsa.pub* is the public key which may be distributed to others who want to identify you.
```
# add public key on the authorized key list at the server
cat ~/.ssh/id_rsa.pub|ssh tamsa1.snu.ac.kr 'xargs -i echo {} >> ~/.ssh/authorized_keys'
```
Now, you can access the server without typing a password.

## Storage
**/home/USER**: The home directories are not shared between different servers. So, we recommend to use shared storage below for analyses.  
**/data6**: For small files such as analysis codes. You can make your directory in */data6/Users/*. **Do not store large files** at *data6*.  
**/data9**: Both for small and large files. You can make your directory in */data9/Users/*. And If you want to use */data9/DATA*, contact with the server administrator.  
**/gv0**: For large files such as ROOT files for data analysis. You can make your directory in */gv0/Users/*. And If you want to use */gv0/DATA*, contact with the server administrator. ,**Do not store small files** at *gv0*.  

## Job scheduler *condor*
@*This section is only relevant for cluster servers (*tamsa1/2*).*  
This section describes briefly about *condor* commands. More detail description can be found in the official [condor manual](https://htcondor.readthedocs.io/en/v9_0/).

### Submitting jobs

#### *condor_submit*
To submit a job, you need a executable script and a submission script.

```
### example of executable: run.sh
#!/bin/bash
hostname
pwd
date
sleep 30
date
```
The executable script should be allowed to execute (`chmod +x run.sh`)
```
### example of submission script: condor.jds
executable = run.sh
output = run.out
error = run.err
log = run.log
queue 1
```
You can submit jobs with `condor_submit condor.jds`. The stdout and stderr will goes to *run.out* and *run.err*, respectively. And *condor* logs will be saved at *run.log*.

#### *condor_run*
This is for very simple jobs. *condor_run* will automatically generate a simple submission script and submit the job. The output and error will be printed as job finished.
```
condor_run "date;pwd;hostname;"
```
This is very convenient for a simple test, but we recommend to use *condor_submit* which is more stable.

### Managing Jobs
#### *condor_q*
This command shows your submitted jobs.  
There are some useful options.  
*-allusers*: print jobs submitted by other users also  
*-global*: print also submitted from other submission machine. **Do not use this option alone**, or it will crash due to a bug. Please use with a username or *-alusers* option.  
*-hold*: print held jobs and the reason  
```
condor_q -al -gl
```

#### *condor_rm*
This command removes jobs from queue.
```
condor_rm USERNAME
```

#### *condor_tail*
This command prints out the stdout (or stderr) of a job currently running.  
```
condor_tail JOBID
```

#### *condor_wait*
This command can be used to wait until the job finished. The condor log file should be provided as a argument.
```
condor_wailt run.log
```

#### *condor_status*
This command shows the status of condor pool.  
There are some useful options.  
*-avail*: print only idle working machines  
*-state*: print available threads and memory  
```
condor_status -avail -state
```
## Advanced usage
### GPU usage
Currently, there are two RTX3090 on *n11* node.   
Here is simple example using *PyTorch*. We need *cuda 11* for RTX3090 and *python 3.8* for *pytorch*.  
```bash
### setup.sh
export PYTHONUSERBASE=/data6/Users/$USER/.local ## tamsa1/2 are not sharing home directory. Let's use /data6 as python user base.
source /cvmfs/sft.cern.ch/lcg/releases/LCG_101cuda/cuda/11.2/x86_64-centos7-gcc8-opt/setup.sh ## cuda 11.2
source /cvmfs/sft.cern.ch/lcg/releases/LCG_101cuda/Python/3.8.6/x86_64-centos7-gcc8-opt/Python-env.sh ## python 3.8.6
source /cvmfs/sft.cern.ch/lcg/releases/LCG_101cuda/pip/21.0.1/x86_64-centos7-gcc8-opt/pip-env.sh ## pip 21.0.1 to instal torch
source /cvmfs/sft.cern.ch/lcg/releases/LCG_101cuda/blas/0.3.17.openblas/x86_64-centos7-gcc8-opt/blas-env.sh ## openblas 0.3.17 for numpy
pip install --user torch==1.8.2+cu111 torchvision==0.9.2+cu111 torchaudio==0.8.2 -f https://download.pytorch.org/whl/lts/1.8/torch_lts.html
```
```python
### gpu_test.py
#!/usr/bin/env python
import torch
 
# Default CUDA device
cuda = torch.device('cuda')
 
# allocates a tensor on default GPU
a = torch.tensor([1., 2.], device=cuda)
 
# transfers a tensor from 'C'PU to 'G'PU
b = torch.tensor([1., 2.]).cuda()

print("cuda avail: {}".format(torch.cuda.is_available()))
print("current device: {}".format(torch.cuda.current_device()))
print("device count: {}".format(torch.cuda.device_count()))

for idx in range(0, torch.cuda.device_count()):
    print("device:{}".format(torch.cuda.device(idx)))
    print("device name:{}".format(torch.cuda.get_device_name(idx)))
```
```bash
### condor.jds
executable=gpu_test.py
output=job.out
error=job.err
log=job.log
getenv=true
request_gpus=1
queue 1
```
You can submit the job with `source setup.sh; condor_submit condor.jds`.

## Additional notes for CMS users
### *ROOT* setup
You can use various versions of *ROOT* from *CVMFS*.
```
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.18.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh
root
```

### CMS grid certificate
Your personal certificate should be registered and installed at SNU servers to use some CMS-specific services.
1. Get personal certificate following the instruction [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/PersonalCertificate).
2. Register your certificate on CMS VO following the intruction [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/).SWGuideLcgAccess#How_to_register_in_the_CMS_VO).
3. Install your certificate in the SNU servers following the instruction [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookStartingGrid).
4. You should be able to generate a proxy by connection to CMS VO.
```
voms-proxy-init -voms cms -valid 192:00
```

### *dasgoclient*
@*You need the [CMS grid certificate](#cms-grid-certificate) to use this service.*  
This is a command line tool for [CMS Data Aggregation System (DAS)](https://cmsweb.cern.ch/das/) which can query useful information of CMS data and simulations.  
```
voms-proxy-init -voms cms
source /cvmfs/cms.cern.ch/cmsset_default.sh
## search samples
dasgoclient --query "/DYJetsToEE*MiNNLO*/RunIISummer20UL16*/MINIAODSIM"
## summary of a sample
dasgoclient --query "summary dataset=/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD" 
## file list of a sample
dasgoclient -limit 5 --query "file dataset=/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD"
```

### *xrootd*
@*You need the [CMS grid certificate](#cms-grid-certificate) to use this service.*  
You can get CMS sample from CMS *xrootd* servers by using *xrdcp* command.  
```
voms-proxy-init -voms cms
xrdcp root://cms-xrd-global.cern.ch//store/data/Run2018A/SingleMuon/MINIAOD/17Sep2018-v2/00000/11697BCC-C4AB-204B-91A9-87F952F9F2C6.root .
```
### CRAB
@*You need the [CMS grid certificate](#cms-grid-certificate) to use this service.*  
You can submit CMS crab jobs from SNU servers.  

### *brilcalc*
You can also use *brilcalc* on SNU servers by using ssh tunneling or web-cache access. I recommend the web-cache access since it is faster although the result is not realtime.
See [brilws document](https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#Off-site-access).
```
##### setup_bril.sh ######
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH ## prerequisite
pip install --user --upgrade brilws ## check update
function _brilcalc_local() { brilcalc $@ -c /cvmfs/cms.cern.ch/SITECONF/local/JobConfig/site-local-config.xml;}                                                                                           
alias brilcalc='_brilcalc_local' ## use the web cache by default

## test ##
brilcalc lumi -r 284077 
```

## Troubleshooting
### My job is held by *condor*
You can check the hold reason using `condor_q -hold`
#### Failed to execute 'run.sh': (errno=13: 'Permission denied')
Check you have the execute permission for the file.  
```
ls -l run.sh
```
You can change the permisson using *chmod* command  
```
chmod +x run.sh
```

#### memory usage exceeded request_memory
Your code exceeded the request_memory (default 3900MB).  
First, check your code whether there is memory leak.  
If you need more memory you can add `request_memory = 8000` in the submission script.  
