CORRECT_FILE='/root/project/MSV/scripts/correct-patch.csv'

def get_correct_config(benchmark:str):
    patch=None
    with open(CORRECT_FILE,'r') as f:
        lines=f.readlines()
        for line in lines:
            cur_benchmark=line.split(',')[0].strip()
            if cur_benchmark==benchmark:
                patch= line.split(',')[1].strip()
                break

    return patch

import json
import os

def get_results(benchmark:str):
    plausibles=[]
    correct=None

    f=open(f'/root/project/MSV-experiment/{benchmark}-out/msv-result.json','r')
    data=json.load(f)
    f.close()

    correct_id=get_correct_config(benchmark)
    for result in data:
        iteration=result['iteration']
        is_plausible=result['pass_result']
        config=result['config'][0]
        patch_id=f'{config["switch"]}-{config["case"]}'

        if 'operator' in config:
            patch_id+=f':{config["operator"]}'
            if 'variable' in config:
                patch_id+=f'-{config["variable"]}-{config["constant"]}'
        
        if is_plausible:
            plausibles.append(patch_id)
        if correct_id is not None and patch_id==correct_id:
            correct=patch_id

    return plausibles,correct

if __name__=="__main__":
    for benchmark in os.listdir('/root/project/MSV-experiment'):
        if benchmark.endswith('-out'):
            cur_benchmark=benchmark[:-4]
            plausibles,correct=get_results(cur_benchmark)
            print(f'Total plausible patches: {len(plausibles)}')
            for plausible in plausibles:
                print(plausible)
            
            if correct is not None:
                print(f'Correct patch found: {correct}')
            else:
                print(f'Correct patch not found')