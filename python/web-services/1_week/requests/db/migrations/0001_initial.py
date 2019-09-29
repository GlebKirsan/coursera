
# Generated by Django 1.11 on 2018-02-04 19:43
from __future__ import unicode_literals

import datetime
from django.db import migrations, models
import django.db.models.deletion
from django.utils.timezone import utc


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Blog',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.CharField(max_length=255)),
                ('created', models.DateTimeField(default=datetime.datetime(2018, 2, 4, 19, 43, 27, 116324, tzinfo=utc))),
            ],
        ),
        migrations.CreateModel(
            name='Topic',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.CharField(max_length=255)),
                ('created', models.DateTimeField(default=datetime.datetime(2018, 2, 4, 19, 43, 27, 117554, tzinfo=utc))),
            ],
        ),
        migrations.CreateModel(
            name='User',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('first_name', models.CharField(max_length=255)),
                ('last_name', models.CharField(max_length=255)),
            ],
        ),
        migrations.AddField(
            model_name='topic',
            name='author',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='db.User'),
        ),
        migrations.AddField(
            model_name='topic',
            name='blog',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='db.Blog'),
        ),
        migrations.AddField(
            model_name='topic',
            name='likes',
            field=models.ManyToManyField(related_name='likes', to='db.User'),
        ),
        migrations.AddField(
            model_name='blog',
            name='author',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='db.User'),
        ),
        migrations.AddField(
            model_name='blog',
            name='subscribers',
            field=models.ManyToManyField(related_name='subscriptions', to='db.User'),
        ),
    ]
